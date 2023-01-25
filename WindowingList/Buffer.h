typedef struct List *list;
typedef struct  node *link;

typedef struct wind{
    int o; //opening
    int c; //closing
}window;

struct node{
    link next; //punta alla finestra successiva
    window w; //finestra
    int **content; //content:(vettore di (e,ts)
    int nc; //numero di oggetti content nella finestra
};

struct List{ //Buffer
    link head; //prima finestra
    int n; //numero totale di finestre
};

list x ;

void allocaBuffer();
void addToBuffer(int e, int ts);
