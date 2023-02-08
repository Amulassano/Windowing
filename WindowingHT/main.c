#include "AlterazioneWindowing.h"
#include <stdio.h>

int main(){

    FILE *ff = fopen("log.txt","w");
    fclose(ff);
    ff = fopen("evict.txt","w");
    fclose(ff);

    char h[1][20];
    for (int i=0;i<1;i++)
        for (int l=0;l<20;l++)
            h[i][l]='h';

    for (int i=1;i<500;i++){
        windowing(h,i);
    }

    return 0;
}
