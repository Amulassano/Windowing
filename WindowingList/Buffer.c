#include <stdlib.h>
#include "Buffer.h"

void allocateBuffer(){ // it allocates the buffer, at the beginning it will be empty and the head will be NULL and the number of elements will be zero
    if (x==NULL) {
        x = malloc(sizeof *x);
        x->head = NULL;
        x->n = 0;
    }
}

static void contentInsert(int e,int ts, link h){ //it insert the content: it knows already the window where to put it
    link p = h;
    p->nc++;
    if (p->content==NULL) { //if the window is empty of content it will allocate space
        p->content = malloc((p->nc) * (sizeof(int *)));
        p->content[p->nc - 1] = malloc(2 * sizeof(int));
    }
    else{ //if in the window there is some content it will increase the allocation space
        p->content = realloc(p->content,(p->nc) * (sizeof(int *)));
        p->content[p->nc - 1] = malloc(2 * sizeof(int));
    }
    p->content[p->nc-1][0] = e; //p->[n][0] it indicates the umpteenth element of that window
    p->content[p->nc-1][1] = ts;//p->[n][1] it indicates the umpteenth timestamp of that window
}

void addToBuffer(int e, int ts){ //add the item to the Buffer
    link p = x->head;
    for (int i=0;i<x->n;i++,p=p->next)
        if (p->w.o<=ts && ts<p->w.c) // it looks for the windows in which the timestamp is inside them
            contentInsert(e,ts,p);
}
