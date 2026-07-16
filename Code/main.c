#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//DEFINIZIONE DI GARAGE E FUNZIONI DI RB

typedef struct veicolo* garage;

struct veicolo{
    int autonomia;
    int quantita;
    garage p;
    garage left;
    garage right;
    char color; //red = 'r', black = 'b'
    garage nil;
};

void svuota_garage(garage *T, garage root);
void stampa_garage(garage T, garage root);
garage cerca_veicolo(garage T, int autonomia, garage root);
garage veicolo_successivo(garage T, int autonomia);
garage veicolo_minimo(garage T, garage root);
garage veicolo_massimo(garage T, garage root);
garage veicolo_precedente(garage T, int autonomia);
void left_rotate(garage *T, garage x);
void right_rotate(garage *T, garage x);
void inserisci_veicolo(garage *T, int autonomia);
void insert_fixup(garage *T, garage z);
void transplant(garage *T, garage u,garage v);
void rottama_veicolo(garage *T, garage *veicolo);
void delete_fixup(garage *T, garage z);

//DEFINIZIONI DI STAZIONI

int numero_stazioni = 0;

struct autorimessa {
    int kilometraggio;
    garage garage;
    struct autorimessa* pepo; //precedente nella visita in ampiezza
    char color; //'w' o 'g' o 'b' -> white o gray o black
    int dist; //distanza nella visita in ampiezza
};

typedef struct autorimessa* stazione;

stazione CANC;

//DEFINIZIONE DI PERCORSO E FUNZIONI

struct tappa {
    stazione key;
    struct tappa* next;
    struct tappa* prev;
};

typedef struct tappa* percorso; 

void inserimento_testa (percorso* l, stazione x);
void rimuovi_tappa (percorso* l);
void stampa_percorso (percorso l);

//DEFINIZIONI E FUNZIONI DI CODA

struct queue_element {
    int cella;
    struct queue_element* next;
    struct queue_element* tail;
};

typedef struct queue_element* queue;

void enqueue (queue* Q, int z);
int dequeue (queue *Q);

//FUNZIONI DI LETTURA

void inizia_lettura(stazione* autostrada);
void riconosci_comando (char* command, stazione* autostrada);

//FUNZIONI DI STAZIONE

int h (int kilometraggio, int i);
void stampa_autostrada (stazione *autostrada);
int cerca_stazione(stazione *autostrada, int kilometraggio);
void aggiungi_auto(int kilometraggio, int autonomia, stazione autostrada);
stazione aggiungi_stazione(int kilometraggio, stazione*  autostrada);
void demolisci_stazione (int toDelete, stazione* autostrada);
void trova_percorso (int partenza, int arrivo, stazione* autostrada);
void confronta_percorsi (percorso* maestro, percorso* allievo);

//SORT

void max_heapify (stazione* A, int i, int heap_size);
void build_max_heap (stazione* A);
void heap_sort (stazione* A);


#define HASHNUM 60000

int main() {
    stazione autostrada[HASHNUM];
    CANC = (stazione)malloc(sizeof(struct autorimessa));
    CANC->garage=NULL;
    CANC->kilometraggio=-1;

    int i=0;
    for (i=0; i<HASHNUM; i++) {
        autostrada[i] = NULL;
    }

    inizia_lettura(autostrada);

    return 0;
}

//FUNZIONI DEI GARAGE

void svuota_garage(garage *T, garage root) {
    if ((*T)!=root->nil) {
        svuota_garage(&((*T)->left), root);
        svuota_garage(&((*T)->right), root);
        free(*T);
    }
}

garage cerca_veicolo(garage T, int autonomia, garage root) {
    if (T==root->nil || autonomia==T->autonomia) {
        return T;
    }
    if (autonomia < T->autonomia) {
        return cerca_veicolo(T->left, autonomia, root);
    }
    else {
        return cerca_veicolo(T->right, autonomia, root);
    }
}

void stampa_garage(garage T, garage root) {
    if (T!=root->nil) {
        stampa_garage(T->left, root);
        if (T->autonomia != -1) {
            printf("autonomia:%d colore:%c quantità:%d ", T->autonomia, T->color, T->quantita);
            if (T->p!=root->nil) {
                printf("padre:%d ", T->p->autonomia);
            }
            if (T->left!=root->nil && T->left->autonomia != -1) {
                printf("left:%d ", T->left->autonomia);
            }
            if (T->right!=root->nil && T->right->autonomia != -1) {
                printf("right:%d ", T->right->autonomia);
            }
            printf("|\n");
        }
        stampa_garage(T->right, root);        
    }
    
}

garage veicolo_minimo(garage T, garage root) {
    garage x;
    x = T;
    while (x->left != root->nil) {
        x=x->left;
    }
    return x;
}

garage veicolo_massimo(garage T, garage root) {
    garage x;
    x = T;
    if (x==root->nil) {
        return x;
    }
    while (x->right != root->nil) {
        x=x->right;
    }
    return x;
}

garage veicolo_successivo(garage T, int autonomia) {
    garage x, y;
    x=cerca_veicolo(T, autonomia, T);
    if (x->right!=T->nil) {
        return veicolo_minimo(x->right, T);
    }
    y = x->p;
    while (y!=T->nil && x==y->right) {
        x = y;
        y = y->p;
    }
    return y;
}

garage veicolo_precedente(garage T, int autonomia) {
    garage x, y;
    x = cerca_veicolo(T, autonomia, T);

    if (x->left != T->nil) {
        return veicolo_massimo(x->left, T);
    }
    y = x->p;
    while (y!=T->nil && x==y->left) {
        x = y;
        y = y->p;
    }
    return y;
}

void left_rotate(garage *T, garage x) {
    garage y;
    
    y=x->right;
    x->right = y->left;
    if (y->left != (*T)->nil) {
        y->left->p = x;
    }
    y->p = x->p;
    if (x->p==(*T)->nil) {
        *T = y;
    }
    else if (x == x->p->left) {
        x->p->left = y;
    }
    else {
        x->p->right = y;
    }
    y->left = x;
    x->p = y;
    
}

void right_rotate(garage *T, garage x) {
    garage y;
    
    y = x->left;
    x->left = y->right;
    if (y->right != (*T)->nil) {
        y->right->p = x;
    }
    y->p = x->p;
    if (x->p==(*T)->nil) {
        *T = y;
    }
    else if (x == x->p->right) {
        x->p->right = y;
    }
    else {
        x->p->left = y;
    }
    y->right = x;
    x->p = y;
}

void inserisci_veicolo(garage *T, int autonomia) {
    garage x, y;

    y = cerca_veicolo(*T, autonomia, *T);
    if (y!=(*T)->nil) {
        y->quantita = y->quantita + 1;
        return;
    }

    garage z = (garage)malloc(sizeof(struct veicolo));


    z->quantita = 1;
    z->autonomia = autonomia;
    z->left = (*T)->nil;
    z->right = (*T)->nil;
    z->p = (*T)->nil;
    z->nil = (*T)->nil;

    y = (*T)->nil;
    x = *T;

    while (x != (*T)->nil) {
        y = x;
        if (z->autonomia<x->autonomia) {
            x = x->left;
        }
        else {
            x = x->right;
        }
    }
    z->p = y;

    if (y == (*T)->nil) {
        *T = z;
    }
    else if (z->autonomia < y->autonomia) {
        y->left = z;
    }
    else {
        y->right = z;
    }
    z->color = 'r';
    insert_fixup(T, z);
}

void insert_fixup(garage *T, garage z) {
    garage x, y;

    if (z == *T) {
        (*T)->color = 'b';
    }
    else {
        x = z->p;
        if (x->color == 'r') {
            if (x == x->p->left){
                y = x->p->right;
                if (y->color == 'r') {
                    x->color = 'b';
                    y->color = 'b';
                    x->p->color = 'r';
                    insert_fixup(T, x->p);
                }
                else {
                    if(z == x->right) {
                        z = x;
                        left_rotate(T, z);
                        x = z->p;
                    }
                    x->color = 'b';
                    x->p->color = 'r';
                    right_rotate(T, x->p);
                }
            }
            else {
                y = x->p->left;
                if (y->color == 'r') {
                    x->color = 'b';
                    y->color = 'b';
                    x->p->color = 'r';
                    insert_fixup(T, x->p);
                }
                else {
                    if(z == x->left) {
                        z = x;
                        right_rotate(T, z);
                        x = z->p;
                    }
                    x->color = 'b';
                    x->p->color = 'r';
                    left_rotate(T, x->p);
                }
            }
        }
    }
}

void rottama_veicolo(garage *T, garage *veicolo) {
    garage x, y, z;

    z = *veicolo;

    if (z->left == (*T)->nil || z->right == (*T)->nil) {
        y = z;
    }
    else {
        y = veicolo_successivo(*T, z->autonomia);
    }
    if (y->left != (*T)->nil) {
        x = y->left;
    }
    else {
        x = y->right;
    }
    x->p = y->p;
    if (y->p == (*T)->nil) {
        *T = x;
    }
    else if (y == y->p->left) {
        y->p->left = x;
    }
    else {
        y->p->right = x;
    }
    if (y!=z) {
        z->autonomia = y->autonomia;
        z->quantita = y->quantita;
    }
    if (y->color == 'b') {
        delete_fixup(T, x);
    }

    free(y);
    return;
}

void delete_fixup(garage *T, garage x) {
    if (x->color == 'r' || x->p == (*T)->nil) {
        x->color = 'b';
    }
    else if (x == x->p->left) {
        garage w;
        w = x->p->right;
        if (w->color == 'r') {
            w->color = 'b';
            x->p->color = 'r';
            left_rotate(T, x->p);
            w = x->p->right;
        }
        if (w->left->color == 'b' && w->right->color == 'b') {
            w->color = 'r';
            delete_fixup(T, x->p);
        }
        else {
            if (w->right->color == 'b') {
                w->left->color = 'b';
                w->color = 'r';
                right_rotate(T, w);
                w = x->p->right;
            }
            w->color = x->p->color;
            x->p->color = 'b';
            w->right->color = 'b';
            left_rotate(T, x->p);
        }
    }
    else {
        garage w;
        w = x->p->left;
        if (w->color == 'r') {
            w->color = 'b';
            x->p->color = 'r';
            right_rotate(T, x->p);
            w = x->p->left;
        }
        if (w->right->color == 'b' && w->left->color == 'b') {
            w->color = 'r';
            delete_fixup(T, x->p);
        }
        else {
            if (w->left->color == 'b') {
                w->right->color = 'b';
                w->color = 'r';
                left_rotate(T, w);
                w = x->p->left;
            }
            w->color = x->p->color;
            x->p->color = 'b';
            w->left->color = 'b';
            right_rotate(T, x->p);
        }
    }
}

//FUNZIONI DI LETTURA

void inizia_lettura(stazione *autostrada) {
    char* command;
    size_t len = 0;
    size_t line_size;

    line_size = getline(&command, &len, stdin);

    while (line_size != EOF) { //LEGGE TUTTI GLI INPUT RIGA PER RIGA E LI SALVA IN COMMAND
        riconosci_comando(command, autostrada);
        line_size = getline(&command, &len, stdin); //LEGGE FINO ALLA PROSSIMA RIGA   
    }

    free(command);
}

void riconosci_comando (char* command, stazione* autostrada) {
    int position=0;
    char istruzione[19];

    while (command[position]!=' ') {
        istruzione[position] = command[position];
        position++;
    }
    istruzione[position] = '\0';

    if (strcmp(istruzione, "aggiungi-stazione") == 0) {
        char string[10];
        int i=0;
        int km=0;
        int numero_auto=0;
        int j=0;
        stazione x;

        position++;
        while (command[position]!=' ') { //kilometraggio stazione
            string[i] = command[position];
            position++;
            i++;
        }
        string[i] = '\0';
        km = atoi(string);
        x = aggiungi_stazione(km, autostrada);
        if (x==NULL) {
            printf("non aggiunta\n");
            return;
        }
        position++;
        i=0;
        while (command[position]!='\n' && command[position] != ' ') {
            string[i] = command[position];
            position++;
            i++;
        }
        string[i] = '\0';
        numero_auto = atoi(string);

        for (j=0; j<numero_auto; j++) {
            int autonomia;
            position++;
            i=0;
            while (command[position]!=' ' && command[position] != '\n') {
                string[i] = command[position];
                position++;
                i++;
            }
            string[i]='\0';
            autonomia = atoi(string);
            inserisci_veicolo(&(x->garage), autonomia);
        }
    }

    else if (strcmp(istruzione, "aggiungi-auto") == 0) {
        int km;
        int autonomia;
        char string[10];
        int i=0;
        int x;
        position++;
        while (command[position]!=' ') {
            string[i] = command[position];
            position++;
            i++;
        }
        string[i] = '\0';
        km = atoi(string);
        x = cerca_stazione(autostrada, km);
        if (x == -1) {
            printf("non aggiunta\n");
            return;
        }
        position++;
        i = 0;
        while (command[position] != '\n') {
            string[i] = command[position];
            position++;
            i++;
        }
        string[i] = '\0';
        autonomia = atoi(string);
        inserisci_veicolo(&(autostrada[x]->garage), autonomia);
        printf("aggiunta\n");
    }

    else if (strcmp(istruzione, "rottama-auto")==0) {
        int km;
        int autonomia;
        char string[10];
        int i=0;
        int x;
        garage y;
        position++;
        while (command[position]!=' ') {
            string[i] = command[position];
            position++;
            i++;
        }
        string[i] = '\0';
        km = atoi(string);
        x = cerca_stazione(autostrada, km);
        if (x == -1) {
            printf("non rottamata\n");
            return;
        }

        position++;
        i = 0;
        while (command[position] != '\n') {
            string[i] = command[position];
            position++;
            i++;
        }
        string[i] = '\0';
        autonomia = atoi(string);
        y = cerca_veicolo(autostrada[x]->garage, autonomia, autostrada[x]->garage);
        if(y == autostrada[x]->garage->nil) {
            printf("non rottamata\n");
            return;
        }
        rottama_veicolo(&(autostrada[x]->garage), &y); 
        printf("rottamata\n");
    }

    else if (strcmp(istruzione, "demolisci-stazione") == 0) {
        char string[10];
        int km;
        int i=0;
        int x;
        position++;
        while (command[position]!='\n') {
            string[i] = command[position];
            position++;
            i++;
        }
        string[i] = '\0';
        km = atoi(string);
        x = cerca_stazione(autostrada, km);
        if (x==-1) {
            printf("non demolita\n");
            return;
        }
        demolisci_stazione(x, autostrada);
        numero_stazioni--;
        printf("demolita\n");
    }

    else if (strcmp(istruzione, "pianifica-percorso")==0) {
        char string[10];
        int i=0;
        int km1=0, km2=0;
        position++;
        while (command[position]!=' ') {
            string[i] = command[position];
            position++;
            i++;
        }
        string[i] = '\0';
        km1 = atoi(string);

        position++;
        i=0;
        while (command[position]!='\n') {
            string[i] = command[position];
            position++;
            i++;
        }
        string[i] = '\0';
        km2 = atoi(string);
        trova_percorso(km1, km2, autostrada);
    }
}

//FUNZIONI DELLE STAZIONI

int h (int kilometraggio, int i) {
    return ((kilometraggio+i)%HASHNUM);
}

void stampa_autostrada(stazione* autostrada) {
    int i=0;
    int num=0;
    while(num!=numero_stazioni-1) {
        if (autostrada[i]!=NULL && autostrada[i]!=CANC) {
            if (veicolo_massimo(autostrada[i]->garage, autostrada[i]->garage)!=autostrada[i]->garage->nil) {
                printf("%d auto:%d ---> ", (autostrada[i])->kilometraggio, (veicolo_massimo(autostrada[i]->garage, autostrada[i]->garage))->autonomia);
            }
            else {
                printf("%d nil ---> ", (autostrada[i])->kilometraggio);
            }
            num++;
        }
        i++;
    }
    while(autostrada[i]==NULL || autostrada[i]==CANC) {
        i++;
    }
    if (veicolo_massimo(autostrada[i]->garage, autostrada[i]->garage)!=autostrada[i]->garage->nil) {
        printf("%d auto:%d\n", (autostrada[i])->kilometraggio, (veicolo_massimo(autostrada[i]->garage, autostrada[i]->garage))->autonomia);
    }
    else {
        printf("%d nil\n", (autostrada[i])->kilometraggio);
    }
}

int cerca_stazione (stazione* autostrada, int kilometraggio) {
    int i = 0;
    int j=0;

    do {
        j = h(kilometraggio, i);
        if (autostrada[j]!=NULL && (autostrada[j])->kilometraggio == kilometraggio) {
            return j;
        }
        else {
            i++;
        }
    } while (i<HASHNUM && autostrada[j]!=NULL);
    return -1;
}

stazione aggiungi_stazione(int kilometraggio, stazione* autostrada) {
    stazione x;
    int check;
    int i = 0;
    int j=0;

    if (numero_stazioni!=0) {
        check = cerca_stazione(autostrada, kilometraggio);
        if (check != -1) {
            return NULL;
        }
    }

    x = (stazione)malloc(sizeof(struct autorimessa));
    x->kilometraggio = kilometraggio;

    garage nil = (garage)malloc(sizeof(struct veicolo)); //SEMPRE INIZIALIZZARE QUESTE 3 RIGHE
    nil->color = 'b'; //ANCHE QUESTA
    nil->autonomia = -1; //ANCHE QUESTA
    nil->nil = nil;
    garage ingresso_garage;
    ingresso_garage = nil;
    ingresso_garage->nil = nil;
    ingresso_garage = ingresso_garage->nil;

    x->garage = ingresso_garage;

    do {
        j = h(kilometraggio, i);
        if (autostrada[j]==NULL) {
            autostrada[j] = x;
            numero_stazioni++;
            printf("aggiunta\n");
            return x;
        }
        else {
            i++;
        }
    } while (i<HASHNUM);
    return NULL;
}

void demolisci_stazione (int toDelete, stazione* autostrada) {
    stazione x = autostrada[toDelete];
    svuota_garage(&(autostrada[toDelete]->garage), autostrada[toDelete]->garage);
    autostrada[toDelete] = CANC;
    free(x);
}

//FUNZIONI DI PERCORSO

void inserimento_testa (percorso* l, stazione x) {
    percorso y = (percorso)malloc(sizeof(struct tappa));
    y->key = x;
    y->next = *l;
    if (*l != NULL) {
        (*l)->prev = y;
    }
    *l = y;
    y->prev = NULL;
}

void rimuovi_tappa (percorso* l) {
    percorso x;
    percorso y;
    x = *l;
    if ((*l)->next == NULL) {
        free(x);
        *l = NULL;
        return;
    }
    y = x->next;
    *l = y;
    (*l)->prev = NULL;
    free(x);
}

void stampa_percorso (percorso l) {
    percorso x;
    x = l;

    while (x->next!=NULL) {
        printf("%d ", x->key->kilometraggio);
        x = x->next;
    }
    printf("%d\n", x->key->kilometraggio);
}

//FUNZIONI DI CODA

void enqueue (queue* Q, int z) {
    queue x;
    x = (queue)malloc(sizeof(struct queue_element));
    x->cella = z;

    if ((*Q)==NULL) {
        x->next = NULL;
        *Q = x;
        (*Q)->tail = x;
        return;
    }
    (*Q)->tail->next = x;
    (*Q)->tail = x;
    x->next = NULL;
}

int dequeue (queue* Q) {
    queue x;
    int ret;
    x = *Q;

    if ((*Q)->next == NULL) {
        ret = (*Q)->cella;
        free(*Q);
        (*Q) = NULL;
        return ret;
    }
    x->next->tail = (*Q)->tail;
    *Q = x->next;
    x->next = NULL;
    ret = x->cella;
    free(x);
    return ret;
}

//SORT

void max_heapify (stazione* A, int i, int heap_size) {
    int l, r, max;
    stazione temp;

    l = (2*i)+1;
    r = (2*i)+2;

    if (l<=heap_size && A[l]->kilometraggio > A[i]->kilometraggio) {
        max = l;
    }
    else {
        max = i;
    }
    if (r<=heap_size && A[r]->kilometraggio>A[max]->kilometraggio) {
        max = r;
    }
    if (max!=i) {
        temp = A[i];
        A[i] = A[max];
        A[max] = temp;
        max_heapify(A, max, heap_size);
    }
}

void build_max_heap (stazione* A) {
    int i;

    for (i = (numero_stazioni-1)/2; i>=0; i--) {
        max_heapify(A, i, numero_stazioni-1);
    }
}

void heap_sort (stazione* A) {
    int i, heap_size;
    stazione temp;
    heap_size = numero_stazioni-1;
    build_max_heap(A);
    for (i=numero_stazioni-1; i>=1; i--) {
        temp = A[0];
        A[0] = A[i];
        A[i] = temp;
        heap_size--;
        max_heapify(A, 0, heap_size);
    }
}

//TROVA PERCORSO

void trova_percorso (int partenza, int arrivo, stazione* autostrada) {
    if (partenza == arrivo) {
        printf("%d\n", partenza);
        return;
    }
    percorso maestro;
    percorso allievo;
    queue Q;
    stazione x;
    int s, check=0, i=0, u, v;
    garage y;
    stazione B[numero_stazioni];
    maestro = NULL;
    allievo = NULL;
    Q = NULL;

    while (check!=numero_stazioni) {
        if (autostrada[i]==NULL || autostrada[i]==CANC) {
            i++;
            continue;
        }
        B[check] = autostrada[i];
        i++;
        check++;
    }
    heap_sort(B);
    

    i=0;
    while ((B[i])->kilometraggio != partenza) {
        i++;
    }
    s = i; 

    for (i=0; i<numero_stazioni; i++) {
        B[i]->color = 'w';
        B[i]->dist = -1;
        B[i]->pepo = NULL;
    }
    
    B[s]->color = 'g';
    B[s]->dist = 0;
    B[s]->pepo = NULL;
    enqueue(&Q, s);
    
    while (Q!=NULL) {
        u = dequeue(&Q);
        if (B[u]->kilometraggio == arrivo) {
            while (Q!=NULL) {
                dequeue(&Q);
            }
            free(Q);
            break;
        }
        y = veicolo_massimo(B[u]->garage, B[u]->garage);
        if (y == B[u]->garage->nil) {
            continue;
        }
        if (partenza < arrivo) {
            v = u+1;
            while (v < numero_stazioni && B[v]->kilometraggio-B[u]->kilometraggio <= y->autonomia) {
                if (B[v]->color == 'w') {
                    B[v]->color = 'g';
                    B[v]->dist = B[u]->dist + 1;
                    B[v]->pepo = B[u];
                    if (B[v]->kilometraggio == arrivo) {
                        x = B[v];
                        while (x->pepo != NULL) {
                            inserimento_testa(&maestro, x);
                            x = x->pepo;
                        }
                        inserimento_testa(&maestro, x);
                    }
                    enqueue(&Q, v);
                }
                else if (B[v]->kilometraggio == arrivo && B[v]->color == 'g' && B[u]->dist < B[v]->dist) {
                    inserimento_testa(&allievo, B[v]);
                    x = B[u];
                    while (x->pepo != NULL) {
                        inserimento_testa(&allievo, x);
                        x = x->pepo;
                    }
                    inserimento_testa(&allievo, x);
                    confronta_percorsi(&maestro, &allievo);
                    while (allievo!=NULL) {
                        rimuovi_tappa(&allievo);
                    }
                }

                v++;
            }
        }
        else if (partenza > arrivo) {
            v = u-1;
            while (v >= 0 && B[u]->kilometraggio - B[v]->kilometraggio <= y->autonomia) {
                if (B[v]->color == 'w') {
                    B[v]->color = 'g';
                    B[v]->dist = B[u]->dist + 1;
                    B[v]->pepo = B[u];
                    if (B[v]->kilometraggio == arrivo) {
                        x = B[v];
                        while (x->pepo != NULL) {
                            inserimento_testa(&maestro, x);
                            x = x->pepo;
                        }
                        inserimento_testa(&maestro, x);
                    }
                    enqueue(&Q, v);
                }
                if (B[v]->kilometraggio != arrivo && B[v]->color == 'g' && B[v]->pepo->kilometraggio > B[u]->kilometraggio && B[v]->dist >= B[u]->dist + 1) {
                    B[v]->pepo = B[u];
                }
                else if (B[v]->kilometraggio == arrivo && B[v]->color == 'g' && B[u]->dist < B[v]->dist) {
                    inserimento_testa(&allievo, B[v]);
                    x = B[u];
                    while (x->pepo != NULL) {
                        inserimento_testa(&allievo, x);
                        x = x->pepo;
                    }
                    inserimento_testa(&allievo, x);
                    confronta_percorsi(&maestro, &allievo);
                    while (allievo!=NULL) {
                        rimuovi_tappa(&allievo);
                    }
                }

                v--;
            }
        }
        B[u]->color = 'b';
    }
    if (maestro == NULL) {
        printf("nessun percorso\n");
    }
    else {
        stampa_percorso(maestro);
        while (maestro!=NULL) {
            rimuovi_tappa(&maestro);
        }
    }
    free(maestro);
    free(allievo);
}

void confronta_percorsi (percorso* maestro, percorso* allievo) {
    percorso m, a;
    a = *allievo;
    m = *maestro;
    
    if (m->key->kilometraggio > m->next->key->kilometraggio) { //partenza > arrivo (365 > 14)
        //controllo il kilometraggio delle stazioni a partire dall'arrivo
        while (m->next!=NULL) {
            m = m->next;
            a = a->next;
        }
        while (m!=NULL) {
            if (a->key->kilometraggio < m->key->kilometraggio) {
                //sostituisci maestro con allievo
                a = *allievo;
                while (a->next!=NULL) {
                    a = a->next;
                }
                while (*maestro!=NULL) {
                    rimuovi_tappa(maestro);
                }
                while (a!=NULL) {
                    inserimento_testa(maestro, a->key);
                    a = a->prev;
                }
                return;
            }
            if (m->key->kilometraggio < a->key->kilometraggio) {
                return;
            }
            m = m->prev;
            a = a->prev;
        }
    }
    else if (m->key->kilometraggio < m->next->key->kilometraggio) { //partenza < arrivo (8 < 298)
        //controllo il kilometraggio delle stazioni a partire dalla partenza
        while (m!=NULL) {
            if (a->key->kilometraggio < m->key->kilometraggio) {
                //sostituisci maestro con allievo
                a = *allievo;
                while (a->next!=NULL) {
                    a = a->next;
                }
                while (*maestro!=NULL) {
                    rimuovi_tappa(maestro);
                }
                while (a!=NULL) {
                    inserimento_testa(maestro, a->key);
                    a = a->prev;
                }
                return;
            }
            if (m->key->kilometraggio < a->key->kilometraggio) {
                return;
            }
            m = m->next;
            a = a->next;
        }
    }
}