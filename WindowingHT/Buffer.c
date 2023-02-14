#include "AlterazioneBuffer.h"
#include <stdlib.h>
#include <math.h>

int t = 0; //initial time
int slid = 5; //initial slide

void allocateBuffer(int ts){ // it allocate the buffer, at the beginning it will be empty and the head will be NULL and the number of elements will be zero
    if (x==NULL) {
        x = malloc(sizeof *x);
        x->head = malloc(Table_Length*(sizeof (x->head)));
        x->N = 0;
        x->M=0;
        x->c = (int) ceil((double) abs(ts- t) / (double) slid)*slid;
        x->count = 0;
    }
}

void addToBuffer(char e[Num_String][String_Length], int ts, int l){ //add the content to the Buffer
    if (x->M>x->N){ //in case N starts again from 0
        while (l<Table_Length) {
            if (ts != x->head[l]->w.c) {
                int k = x->N;
                x->head[l]->nc++;
                if (x->head[l]->c ==
                    NULL) { //if the window is empty of content i will allocate space
                    x->head[l]->c = malloc(sizeof(*x->head[l]->c));

                } else { //if in the window there is some content i will increase the allocation space
                    x->head[l]->c = realloc(x->head[l]->c, (x->head[l]->nc) * (sizeof(*x->head[l]->c)));

                }
                x->head[l]->c[x->head[l]->nc - 1].e = malloc(Num_String*sizeof(char*));
                for (int i=0;i<Num_String;i++)
                    x->head[l]->c[x->head[l]->nc - 1].e[i] = malloc(String_Length*sizeof(char));
                for (int p=0;p<Num_String;p++)
                    for (int t=0;t<String_Length;t++)
                        x->head[l]->c[x->head[l]->nc - 1].e[p][t] = e[p][t]; //it indicates the umpteenth element of that window
                x->head[l]->c[x->head[l]->nc - 1].ts = ts;//it indicates the umpteenth timestamp of that window
            }l++;
        }
        l = 0;
        while (l<x->N) {
            if (ts != x->head[l]->w.c) {
                x->head[l]->nc++;
                if (x->head[l]->c ==
                    NULL) { //if the window is empty of content i will allocate space
                    x->head[l]->c = malloc(sizeof(*x->head[l]->c));
                } else { //if in the window there is some content i will increase the allocation space
                    x->head[l]->c = realloc(x->head[l]->c, (x->head[l]->nc) * (sizeof(*x->head[l]->c)));

                }
                x->head[l]->c[x->head[l]->nc - 1].e = malloc(Num_String*sizeof(char*));
                for (int i=0;i<Num_String;i++)
                    x->head[l]->c[x->head[l]->nc - 1].e[i] = malloc(String_Length*sizeof(char));
                for (int p=0;p<Num_String;p++)
                    for (int t=0;t<String_Length;t++)
                        x->head[l]->c[x->head[l]->nc - 1].e[p][t] = e[p][t]; //it indicates the umpteenth element of that window
                x->head[l]->c[x->head[l]->nc - 1].ts = ts;//it indicates the umpteenth timestamp of that window
            }l++;

        }
    }
    else {
        while (l < x->N) {
            if (ts != x->head[l]->w.c) {
                x->head[l]->nc++;
                if (x->head[l]->c ==
                    NULL) { //if the window is empty of content i will allocate space
                    x->head[l]->c = malloc(sizeof(*x->head[l]->c));
                } else { //if in the window there is some content i will increase the allocation space
                    x->head[l]->c = realloc(x->head[l]->c, (x->head[l]->nc) * (sizeof(*x->head[l]->c)));
                }
                x->head[l]->c[x->head[l]->nc - 1].e = malloc(Num_String*sizeof(char*));
                for (int i=0;i<Num_String;i++)
                    x->head[l]->c[x->head[l]->nc - 1].e[i] = malloc(String_Length*sizeof(char));
                for (int p=0;p<Num_String;p++)
                    for (int t=0;t<String_Length;t++)
                        x->head[l]->c[x->head[l]->nc - 1].e[p][t] ='s';// e[p][t]; //it indicates the umpteenth element of that window
                x->head[l]->c[x->head[l]->nc - 1].ts = ts;//it indicates the umpteenth timestamp of that window
            }l++;
        }
    }
}


