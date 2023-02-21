#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Buffer.h"

int t0 = 0; //initial time
int slide = 2; //slide of a widnow
int width = 2; //width of a window
int tprev = 0; //previous time


link newNode(int *content, int o,int c, link next){ // it creates a new node in the list
    link p = malloc(sizeof *p);
    if (p == NULL)
        return NULL;
    else {
        p->next = next; //next indicates the following node
        p->w.c = c; //c = window closure
        p->w.o = o; //o = window opening
        p->nc = 0; //it indicates the number of item in that window
        p->content = NULL; //in the creation of the node the content doesn't exist yet
        if (content != NULL) {
            p->nc++;
            p->content = malloc(sizeof (int*)); //it allocates the content as a matrix of two dimensions: the first represents the element and the second the timestamp
            p->content[0] = malloc(2*sizeof (int));
            p->content[0][0] = content[0];
            p->content[0][1] = content[0];
        }
    }
    return p;
}

link listInsTail(link h,int *content, int o,int c){ //it inserts the node in the tail: useful for the scope
    link p;
    if (h==NULL)
        return newNode(content,o,c,NULL);
    for (p=h; p->next != NULL ; p= p->next);//it looks for the last node in a sequentially way in order to create a new one
    p->next = newNode(content,o,c,NULL);
    return h;
}

link scope(int ts){ // scope function
    int c = (int) ceil((double) abs(ts- t0) / (double) slide)*slide; //it computes the closure of the first window
    int o = c - width; //it computes the opening of the first window
    link p = x->head;
    if (p==NULL) { //if there aren't any windows yet, it creates them 
        printf("Calculating the Windows to Open. First one opens at [ %d ] and closes at [ %d ]\n", o, c);

        do { //it computes all the windows, from the first, till to my current timestap
            printf("Computing window [ %d , %d ) if absent\n", o, o + width);
            x->n++;
            p = listInsTail(p, NULL, o, o + width);
            o += slide;

        } while (o <= ts);
    }
    else{ //it computes the new windows and adds them to the previous ones
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

int tick(int tau, int ts){ //tick function: it returns 1 ( TRUE ) if the current timestamp it's greater than or equal to the previous one ( tau ), otherwise it returns 0 ( FALSE )
    if (ts>=tau)
        return 1;
    return 0;
}

void extractData(link h,int ***cont){ //it extracts a content vector
    int **p;
    if (h->nc!=0) {
        p = malloc(h->nc * (sizeof(int *)));
        for (int i = 0; i < h->nc;i++){
            p[i] = malloc(2*(sizeof (int)));
            p[i][0] = h->content[i][0]; //it extracts the element
            p[i][1] = h->content[i][1]; //it extracts its timestamp
        }
        *cont=p;
    }
    else
        return ;
}

int active(window w, int ts){ //it checks if the window is activate: if the current timestamp is in between the window extremes it is activate and it returns 1, otherwise it's not activate and it returns 0
    if (w.o<=ts && ts<= w.c)
        return 1;
    return 0;
}

int report(window w, int ts){ //report: window_close ( it returns TRUE if timestamp = window closure )
    if (w.c==ts)
        return 1;
    return 0;
}

link evictWindow(int ts){ //delete the window
    FILE *ff;
    ff = fopen("evict.txt","a");
    link p = x->head;
    fprintf(ff,"Evicting [ %d, %d ], ts= %d\n",p->w.o,p->w.c,ts);
    if (x->head==NULL) //if there are no windows
        return NULL;
    x->head = x->head->next;
    if (p->nc>0) { //if some content is inside it will free that space
        for (int i = 0; i < p->nc; i++)
            free(p->content[i]);
        free(p->content);
    }
    free(p);
    x->n--; //it decreases the amount of windows in the Buffer
    fclose(ff);
    return x->head;
}

void compute(link h, int **content){ //it prints the content
    FILE *fp;
    fp = fopen("log.txt","a");
    if (h->nc>0) { //if there is some content
        fprintf(fp,"%d, %d, %d",tprev,h->w.o,h->w.c);
        for (int i = 0; i < h->nc; i++)
            fprintf(fp, ", < %d, %d >", content[i][0], content[i][1]);
        fprintf(fp,"\n");
    }
    fclose(fp);
}

void windowing(int e, int ts){

    allocateBuffer(); //

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
