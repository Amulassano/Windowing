#define Table_Length 100000
#define String_Length 20
#define Num_String 1

typedef struct List *list;
typedef struct  node *link;

typedef struct wind{ // [o,c)
    int o; //opening
    int c; //closing -> opened
}window;

typedef struct content{
    char **e;
    int ts;
}content;

struct node{
    window w;
    content *c; //vector of (e,ts)
    int nc; //number of contents in the window

};

struct List{ //Buffer
    link *head; //first window
    int N; // it represents the first ceil free after the last one
    int M; // it represents the first line occupied
    int c; //first closure
    int count; //number of ceils passed since the beginning
};

list x;


void allocateBuffer(int ts);
void addToBuffer(char e[Num_String][String_Length], int ts, int l);
