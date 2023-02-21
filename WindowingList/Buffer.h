typedef struct List *list;
typedef struct  node *link;

typedef struct wind{
    int o; //opening
    int c; //closing
}window;

struct node{
    link next; //it's linked to the following window
    window w; 
    int **content; //content:(vector of (e,ts)
    int nc; //number of item in the window
};

struct List{ //Buffer
    link head; //first window
    int n; //number of windows
};

list x ;

void allocateBuffer();
void addToBuffer(int e, int ts);
