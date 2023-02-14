#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "AlterazioneBuffer.h"

int slide = 5; //initial slide
int width = 50; //initial width
int tprev = 0; //previous time

typedef struct{ //Data interface type
    char **e;
    int ts;
}data;

link newNode( int o,int c){ //it creates a new node in the list
    link p = malloc(sizeof *p);
    p->w.c = c; //c = window closure
    p->w.o = o; //o = window opening
    p->nc = 0; // it indicates the number of contents in the considered window
    p->c = NULL; //in the creation of the node the content doesn't exist yet
    return p;
}

void evictWindow(int ts,int l){ //delete the window
    FILE *ff;
    ff = fopen("evict.txt","a");
    fprintf(ff,"Evicting [ %d, %d ), ts= %d\n",x->head[l]->w.o,x->head[l]->w.c,ts);
    if (x->head[l]->nc>0) { //if some content is inside it will free that space
        for (int i=0;i<x->head[l]->nc;i++) {
            for (int f = 0; f < Num_String; f++)
                free(x->head[l]->c[i].e[f]);
            free(x->head[l]->c[i].e);
        }

        free(x->head[l]->c);
        x->head[l]->c=NULL;
        x->head[l]->nc=0;
    }
    x->M++; //it decreases the amount of windows in the Buffer
    if (x->M >=Table_Length)
        x->M -= Table_Length;
    fclose(ff);
}

int giveMeLast(){ //it returns the position of the last open window if the table was infinite
    if (x->count==0)
        return 0;
    return (int)ceil((double)(tprev-x->c)/(double)slide);
}

void TakeAllOff(int pos){ //it frees the whole table
    x->M =pos%Table_Length;
    x->N =(pos)%Table_Length;
    free(x->head);
    x->head = malloc(Table_Length*(sizeof (x->head)));
}

int chooseIfSkip(int ts){
    int l = (int)ceil((double)(ts-x->c)/(double)slide);
    int last = giveMeLast();
    if (l-last<Table_Length)
        return 1;
    return 0;
}

int scope(int ts){ //scope function: it returns the exactly position in which to put (e,ts)
    int c = x->c;
    int o = c - width; //it computes the opening of the very first window
    int i = 0;
    int l;
    l = (int)ceil((double)(ts-c)/(double)slide); //hash
    int last = giveMeLast();
    if (chooseIfSkip(ts)) {
        l= l%Table_Length; //to be sure the position will be in the range of the table length
        if (x->count == 0) {
            printf("Calculating the Windows to Open. First one opens at [ %d ] and closes at [ %d ],ts=%d\n", o, c, ts);
            do { //it computes all thewindows, from the first, till to my current timestap
                printf("Computing window [ %d , %d ) if absent\n", o, o + width);
                x->N++;
                x->count++;
                if (x->N >= Table_Length) //if the last ceil overcome the width of the table, the count will restart from zero
                    x->N = x->N - Table_Length;
                x->head[i] = newNode(o, o + width);
                o += slide;
                i++;
            } while (o <= ts);
        } else {
            printf("Calculating the Windows to Open. First one opens at [ %d ] and closes at [ %d ],ts=%d\n",
                   x->head[x->M]->w.o, x->head[x->M]->w.c, ts);
            //it computes the new windows and adds them to the previous ones

            if (x->N == 0) {
                o = x->head[Table_Length - 1]->w.o + slide;
                i = 0;
            } else {
                i = x->N;
                o = x->head[x->N - 1]->w.o + slide;
            }
            if (o <= ts) {
                while (o <= ts) {
                    printf("Computing window [ %d , %d ) if absent\n", o, o + width);
                    x->N++;
                    x->count++;
                    if (x->N >= Table_Length) //if the last ceil overcome the width of the table, the count will restart from zero
                        x->N = x->N - Table_Length;
                    x->head[i] = newNode(o, o + width);
                    o += slide;
                    i++;
                    if (i >= Table_Length) //the counter restarts from the first ceil
                        i -= Table_Length;
                }
            }
        }
    }
    else{
        last = last%Table_Length;
        int ch = x->head[last]->w.c;
        while (ch<ts)
            ch += width;
        TakeAllOff(l);
        o = ch-width;
        i = l%Table_Length;
        printf("Calculating the Windows to Open. First one opens at [ %d ] and closes at [ %d ],ts=%d\n", o, ch, ts);
        do { //it computes all thewindows, from the first, till to my current timestap
            printf("Computing window [ %d , %d ) if absent\n", o, o + width);
            x->N++;
            x->count++;
            if (x->N >= Table_Length) //if the last ceil overcome the width of the table, the count will restart from zero
                x->N = x->N - Table_Length;
            x->head[i] = newNode(o, o + width);
            o += slide;
            i++;
            i = i%Table_Length;
        } while (o <= ts);
    }
    return l;
}

int tick(int tau, int ts){ //tick function: it returns 1 ( TRUE ) if the current timestamp it's greater than or equal to the previous one ( tau ), otherwise it returns 0 ( FALSE )
    if (ts>=tau)
        return 1;
    return 0;
}

void extractData(link h,data *cont){ //it extracts a content vector
    if (h->nc!=0) {
        for (int i = 0; i < h->nc;i++){
            for (int l=0;l<Num_String;l++)
                for (int t=0;t<String_Length;t++)
                    cont[i].e[l][t] = h->c[i].e[l][t]; //it extracts the last element
            cont[i].ts = h->c[i].ts; //it extracts its timestamp
        }
    }
    else
        return ;
}

int active(window w, int ts){ // it checks if the window is activate: if the current timestamp is in between the window extremes it is activate and it returns 1, otherwise it's not activate and it returns 0
    if (w.o<=ts && ts<= w.c)
        return 1;
    return 0;
}

int report(window w, int ts){ //report: window_close ( it returns TRUE if timestamp = window closure )
    if (w.c==ts)
        return 1;
    return 0;
}

void compute(link h, data *content){ //it prints the content
    FILE *fp;
    fp = fopen("log.txt","a");
    if (h->nc>0) { //if there is some content
        fprintf(fp,"%d, %d, %d",tprev,h->w.o,h->w.c);

        for (int i = 0; i < h->nc; i++) {
            for (int l = 0; l < Num_String; l++) {
                for (int p = 0; p < String_Length; p++) {
                    if (l == 0 && p==0)
                        fprintf(fp, ", < %c", content[i].e[l][p]);
                    else
                        fprintf(fp, "%c", content[i].e[l][p]);
                }
            }
            fprintf(fp, ", %d >", content[i].ts);
        }
        fprintf(fp,"\n");
    }
    fclose(fp);
}

void windowing(char e[Num_String][String_Length], int ts){
    int l;

    data *cont = NULL;

    allocateBuffer(ts); //it allocates the buffer

    if (ts>=tprev) {

        l = scope(ts); //it finds the index of the ceil in which to insert the content


        addToBuffer(e, ts, l); //add the content

        if (tick(tprev, ts) && chooseIfSkip(ts)==1) {
            int k = x->N;
            if (x->N > x->M) { // two cases: N>M -> it can iter in the table without problems; N<M -> it means that there is a need of two cycles: the first goes up to the table length and the second starts from zero and goes up to N
                for (int i = x->M; i < k; i++) {
                    if (active(x->head[i]->w, ts)) {
                        cont=malloc(x->head[i]->nc * (sizeof(data)));
                        for (int l=0;l<x->head[i]->nc;l++) {
                            cont[l].e = malloc(Num_String * (sizeof(char *)));
                            for (int y = 0; y < Num_String; y++)
                                cont[l].e[y] = malloc(String_Length * (sizeof(char)));
                        }
                        extractData(x->head[i], cont);
                        if (report(x->head[i]->w, ts)) {
                            compute(x->head[i], cont);
                        }
                        for (int l=0;l<x->head[i]->nc;l++) {
                            for (int y = 0; y < Num_String; y++)
                                free(cont[l].e[y]);
                            free(cont[l].e);
                        }
                        free(cont);
                    }

                }
                for (int i = x->M; i < k; i++) {
                    if (x->head[i]->w.c <= ts) {
                        evictWindow(ts, i);
                    }
                    else
                        break;
                }
            } else {
                for (int i = x->M; i < Table_Length; i++) { //first cycle
                    if (active(x->head[i]->w, ts)) {
                        cont=malloc(x->head[i]->nc * (sizeof(data)));
                        for (int l=0;l<x->head[i]->nc;l++) {
                            cont[l].e = malloc(Num_String * (sizeof(char *)));
                            for (int y = 0; y < Num_String; y++)
                                cont[l].e[y] = malloc(String_Length * (sizeof(char)));
                        }
                        extractData(x->head[i], cont);
                        if (report(x->head[i]->w, ts)) {
                            compute(x->head[i], cont);
                        }
                        for (int l=0;l<x->head[i]->nc;l++) {
                            for (int y = 0; y < Num_String; y++)
                                free(cont[l].e[y]);
                            free(cont[l].e);
                        }
                        free(cont);
                    }
                }
                for (int i = x->M; i < Table_Length; i++) {
                    if (x->head[i]->w.c <= ts) {
                        evictWindow(ts, i);
                    } else
                        break;
                }
                if (k != 0) { //second cycle
                    for (int i = 0; i < k; i++) {
                        if (active(x->head[i]->w, ts)) {
                            cont=malloc(x->head[i]->nc * (sizeof(data)));
                            for (int l=0;l<x->head[i]->nc;l++) {
                                cont[l].e = malloc(Num_String * (sizeof(char *)));
                                for (int y = 0; y < Num_String; y++)
                                    cont[l].e[y] = malloc(String_Length * (sizeof(char)));
                            }
                            extractData(x->head[i], cont);
                            if (report(x->head[i]->w, ts)) {
                                compute(x->head[i], cont);
                            }
                            for (int l=0;l<x->head[i]->nc;l++) {
                                for (int y = 0; y < Num_String; y++)
                                    free(cont[l].e[y]);
                                free(cont[l].e);
                            }
                            free(cont);
                        }
                    }
                    for (int i = 0; i < k; i++) {
                        if (x->head[i]->w.c <= ts) {
                            evictWindow(ts, i);
                        }
                        else
                            break;
                    }
                }
            }
        }
        tprev = ts;
    }
}
