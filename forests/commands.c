#include <stdio.h>
#include "commands.h"
#include "def.h"

/* W implemencji wszystkich poniższych funkcji zakładam, że dane są poprawne */

void add(Tree *t, int *Arg[]) {
    int i = 0;
    Tree tmp;
    while (i < MAX_ARG && Arg[i] != NULL) {
        tmp = insert(t, Arg[i]);
        t = &tmp->elements;
        i++;
    }
    printf("OK\n");
}

void del(Tree *t, int *Arg[]) {
    int i = 0; //iterator i count
    Tree tmp = *t;
    Tree *el = t;

    if (Arg[0] == NULL) {
        removeAll(*t);
        *t = NULL;
    }

    while (i < MAX_ARG && Arg[i] != NULL && tmp != NULL) {
        if (i + 1 == MAX_ARG || Arg[i + 1] == NULL) {
            removeElement(el, Arg[i]);
        } else {
            tmp = find(*el, Arg[i]);
            if (tmp)
                el = &tmp->elements;
        }
        i++;
    }
    printf("OK\n");
    return;
}

void print(Tree t, int *Arg[]) {
    int i = 0;
    while (i < MAX_ARG && Arg[i] != NULL) {
        t = find(t, Arg[i]);
        if (t)
            t = t->elements;
        i++;
    }
    printBST(t);
}

static void searchAll(Tree t, int *Arg[], int i, int *found) {
    Tree tmp;
    if (t && !*found) {
        if (Arg[i] != NULL && Arg[i][0] == '*' &&
            Arg[i][1] == '\0') { //jeśli argument jest gwiazdką przeszukuje wszystkie elementy danego drzewa
            searchAll(t->left, Arg, i, found);
            searchAll(t->right, Arg, i, found);
            searchAll(t->elements, Arg, i + 1, found);
        } else if (i < MAX_ARG && Arg[i] != NULL) {
            tmp = find(t, Arg[i]);
            if (tmp)
                searchAll(tmp->elements, Arg, i + 1, found);
            return;
        }
    }
    if (i == MAX_ARG || Arg[i] == NULL) //element znaleziony - doszliśmy do końca tablicy argumentów, czyli znaleźliśmy wszystkie poprzednie
        *found = 1;
    return;
}

void check(Tree t, int *Arg[]) {
    int found = 0;
    searchAll(t, Arg, 0, &found);
    if (found)
        printf("YES\n");
    else
        printf("NO\n");
}
