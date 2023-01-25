#include <stdlib.h>
#include "Buffer.h"

void allocaBuffer(){ //alloca il Buffer, che inizialmente sarà vuoto e dunque la testa sarà = NULL e il numero di elementi presenti sarà zero
    if (x==NULL) {
        x = malloc(sizeof *x);
        x->head = NULL;
        x->n = 0;
    }
}

static void contentInsert(int e,int ts, link h){ //inserisce il content: conosco già la finestra in cui metterlo
    link p = h;
    p->nc++;
    if (p->content==NULL) { //se nella finestra non è già presente un conenuto allora vado ad allocare lo spazio da zero
        p->content = malloc((p->nc) * (sizeof(int *)));
        p->content[p->nc - 1] = malloc(2 * sizeof(int));
    }
    else{ //se è già presente del contenuto aumento lo spazio dell'allocazione dinamica
        p->content = realloc(p->content,(p->nc) * (sizeof(int *)));
        p->content[p->nc - 1] = malloc(2 * sizeof(int));
    }
    p->content[p->nc-1][0] = e; //p->[n][0] indica l'ennesimo content di quella finestra
    p->content[p->nc-1][1] = ts;//p->[n][1] indica l'ennesimo timestamp di quella finestra
}

void addToBuffer(int e, int ts){ //aggiunge al Buffer
    link p = x->head;
    for (int i=0;i<x->n;i++,p=p->next)
        if (p->w.o<=ts && ts<p->w.c) //cerco le finestre in cui il timestamp sia all'interno
            contentInsert(e,ts,p);
}
