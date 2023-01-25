#include "Buffer.h"
#include <stdlib.h>
#include <math.h>

int t = 0; //scelgo un tempo iniziale
int slid = 2; //scelgo un slide per le finestre

void allocaBuffer(int ts){ //alloca il Buffer, che inizialmente sarà vuoto e dunque la testa sarà = NULL e il numero di elementi presenti sarà zero
    if (x==NULL) {
        x = malloc(sizeof *x);
        x->head = malloc(MM*(sizeof (x->head)));
        x->N = 0;
        x->M=0;
        x->c = (int) ceil((double) abs(ts- t) / (double) slid)*slid;
        x->count = 0;
    }
}

void addToBuffer(char e[String_Lenght], int ts, int l){ //aggiunge al Buffer
    if (x->M>x->N){ //nel momento in cui N riparte da 0
        while (l<MM) {
            if (ts != x->head[l]->w.c) {
                int k = x->N;
                x->head[l]->nc++;
                if (x->head[l]->c ==
                    NULL) { //se nella finestra non è già presente un conenuto allora vado ad allocare lo spazio da zero
                    x->head[l]->c = malloc(sizeof(*x->head[l]->c));

                } else { //se è già presente del contenuto aumento lo spazio dell'allocazione dinamica
                    x->head[l]->c = realloc(x->head[l]->c, (x->head[l]->nc) * (sizeof(*x->head[l]->c)));

                }
                for (int t=0;t<String_Lenght;t++)
                    x->head[l]->c[x->head[l]->nc - 1].e[t] = e[t]; //p->[n][0] indica l'ennesimo content di quella finestra
                x->head[l]->c[x->head[l]->nc - 1].ts = ts;//p->[n][1] indica l'ennesimo timestamp di quella finestra
            }l++;
        }
        l = 0;
        while (l<x->N) {
            if (ts != x->head[l]->w.c) {
                x->head[l]->nc++;
                if (x->head[l]->c ==
                    NULL) { //se nella finestra non è già presente un conenuto allora vado ad allocare lo spazio da zero
                    x->head[l]->c = malloc(sizeof(*x->head[l]->c));
                } else { //se è già presente del contenuto aumento lo spazio dell'allocazione dinamica
                    x->head[l]->c = realloc(x->head[l]->c, (x->head[l]->nc) * (sizeof(*x->head[l]->c)));
                }
                for (int t=0;t<String_Lenght;t++)
                    x->head[l]->c[x->head[l]->nc - 1].e[t] = e[t]; //p->[n][0] indica l'ennesimo content di quella finestra
                x->head[l]->c[x->head[l]->nc - 1].ts = ts;//p->[n][1] indica l'ennesimo timestamp di quella finestra
            }l++;

        }
    }
    else {
        while (l < x->N) {
            if (ts != x->head[l]->w.c) {
                x->head[l]->nc++;
                if (x->head[l]->c ==
                    NULL) { //se nella finestra non è già presente un conenuto allora vado ad allocare lo spazio da zero
                    x->head[l]->c = malloc(sizeof(*x->head[l]->c));
                } else { //se è già presente del contenuto aumento lo spazio dell'allocazione dinamica
                    x->head[l]->c = realloc(x->head[l]->c, (x->head[l]->nc) * (sizeof(*x->head[l]->c)));
                }
                for (int t=0;t<String_Lenght;t++)
                    x->head[l]->c[x->head[l]->nc - 1].e[t] = e[t]; //p->[n][0] indica l'ennesimo content di quella finestra
                x->head[l]->c[x->head[l]->nc - 1].ts = ts;//p->[n][1] indica l'ennesimo timestamp di quella finestra
            }l++;
        }
    }
}
