#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Buffer.h"

int t0 = 0; //scelgo un tempo iniziale
int slide = 2; //scelgo un slide per le finestre
int width = 2; //scelgo una larghezza per le finestre
int tprev = 0; //previous time


link newNode(int *content, int o,int c, link next){ // mi crea un nuovo nodo nella lista
    link p = malloc(sizeof *p);
    if (p == NULL)
        return NULL;
    else {
        p->next = next; //next indica il nodo successivo
        p->w.c = c; //c = chiusura della finestra
        p->w.o = o; //o = apertura della finestra
        p->nc = 0; //indica il numero di contenuti all'interno della finestra considerata
        p->content = NULL; //al momento della creazione di un nodo il contenuto non è esistente
        if (content != NULL) {
            p->nc++;
            p->content = malloc(sizeof (int*)); //alloco il contenuto come una matrice di vettori di dimensione 2: la prima rappresenta l'elemento e la seconda il timestamp
            p->content[0] = malloc(2*sizeof (int));
            p->content[0][0] = content[0];
            p->content[0][1] = content[0];
        }
    }
    return p;
}

link listInsTail(link h,int *content, int o,int c){ //inserisce in coda una lista: utile per lo scope
    link p;
    if (h==NULL)
        return newNode(content,o,c,NULL);
    for (p=h; p->next != NULL ; p= p->next);//ricerca in mood sequenziale l'ultimo nodo per crearne uno nuovo successivamente
    p->next = newNode(content,o,c,NULL);
    return h;
}

link scope(int ts){ //funzione scope
    int c = (int) ceil((double) abs(ts- t0) / (double) slide)*slide; //calcolo la chiusura della prima finestra
    int o = c - width; //calcolo l'apertura della prima finestra
    link p = x->head;
    if (p==NULL) { //se non sono ancora presenti finestre le aggiunge da zero
        printf("Calculating the Windows to Open. First one opens at [ %d ] and closes at [ %d ]\n", o, c);

        do { //ciclo che calcola, dalla prima finestra, tutte le finestre fino al mio timestamp attuale
            printf("Computing window [ %d , %d ) if absent\n", o, o + width);
            x->n++;
            p = listInsTail(p, NULL, o, o + width);
            o += slide;

        } while (o <= ts);
    }
    else{ //calcola le nuove finestre e le aggiunge a quelle già presenti
        link l=p;
        while (l->next!=NULL)
            l=l->next;
        if (l->w.c<=ts) {
            o = l->w.c;
            do {
                printf("Computing window [ %d , %d ) if absent\n", o, o + width);
                x->n++;
                l = listInsTail(l, NULL, o, o + width);
                o += slide;

            } while (o <= ts);
        }
    }
    return p;
}

int tick(int tau, int ts){ //funzione tick: ritorna 1 ( ovvero TRUE ) se il timestamp attuale è maggiore o uguale a quello precedente ( tau ), altrimenti 0 ( FALSE )
    if (ts>=tau)
        return 1;
    return 0;
}

void extractData(link h,int ***cont){ //mi estrae un vettore di content (e,ts)
    int **p;
    if (h->nc!=0) {
        p = malloc(h->nc * (sizeof(int *)));
        for (int i = 0; i < h->nc;i++){
            p[i] = malloc(2*(sizeof (int)));
            p[i][0] = h->content[i][0]; //estraggo l'elemento
            p[i][1] = h->content[i][1]; //estraggo il relativo timestamp
        }
        *cont=p;
    }
    else
        return ;
}

int active(window w, int ts){ //controllo se la finestra è attiva o meno: se l'attuale timestamp è compreso tra gli estremi della finestra allora essa è attiva e restituisce 1, altrimenti no e restituisce zero
    if (w.o<=ts && ts<= w.c)
        return 1;
    return 0;
}

int report(window w, int ts){ //report: window_close ( restituisce ( TRUE ) se timestamp = chiusura della finestra )
    if (w.c==ts)
        return 1;
    return 0;
}

link evictWindow(int ts){ //elimina la finestra
    FILE *ff;
    ff = fopen("evict.txt","a");
    link p = x->head;
    fprintf(ff,"Evicting [ %d, %d ], ts= %d\n",p->w.o,p->w.c,ts);
    if (x->head==NULL) //se finestra già inesistente
        return NULL;
    x->head = x->head->next;
    if (p->nc>0) { //se è presente del content all'interno vado a liberare lo spazio allocatogli precedentemente
        for (int i = 0; i < p->nc; i++)
            free(p->content[i]);
        free(p->content);
    }
    free(p);
    x->n--; //diminuisco nel Buffer il totale delle finestre presenti
    fclose(ff);
    return x->head;
}

void compute(link h, int **content){ //stampa il content
    FILE *fp;
    fp = fopen("log.txt","a");
    if (h->nc>0) { //se è presente del content
        fprintf(fp,"%d, %d, %d",tprev,h->w.o,h->w.c);
        for (int i = 0; i < h->nc; i++)
            fprintf(fp, ", < %d, %d >", content[i][0], content[i][1]);
        fprintf(fp,"\n");
    }
    fclose(fp);
}

void windowing(int e, int ts){

    allocaBuffer(); //

    if (ts>=tprev)
        x->head = scope(ts);

    addToBuffer(e, ts); //

    if (tick(tprev,ts)) {
        link h=x->head;
        int l = x->n;
        for (int i=0;i<l;i++) {
            if (active(h->w, ts)) {
                int **cont;
                extractData(h, &cont);
                if (report(h->w, ts)) {
                    compute(h, cont);
                }
            }
            h = h->next;
        }
        h=x->head;
        for (int i=0;i<l;i++){
            if (h->w.c<=ts) {
                x->head = evictWindow(ts);
                h = x->head;
                }
        }
        tprev= ts;
    }
}
