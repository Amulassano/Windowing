#include "Windowing.h"
#include <stdio.h>

int main(){
    FILE *f = fopen("input.txt","r");
    int e,ts;

    while (!(feof(f))){
        fscanf(f,"%d %d",&e,&ts);
        windowing(e,ts);
    }

    return 0;
};
