#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

/* wypisuje słowo znajdujące się w tablicy A */
static void printArray(int A[]) {
    int i = 0;
    while (A[i] != '\0') {
        printf("%c", A[i]);
        i++;
    }
    printf("\n");
}

/* Znajduje i zwraca najmniejszy element w BST */
static Tree findMin(Tree *t) {
    Tree tmp = *t;
    if (tmp->left == NULL) {
        *t = tmp->right;
        return tmp;
    } else
        return findMin(&tmp->left);
}

/* Porównuje dwie tablice int'ów, zwraca -1 jeśli pierwsza jest leksykograficznie mniejsza, 1 jeśli większa i 0 jeśli są takie same */
static int compareNames(int A[], int B[]) {
    int i = 0;
    while (A[i] != '\0' && B[i] != '\0' && A[i] == B[i])
        i++;
    if (A[i] < B[i])
        return -1;
    if (A[i] > B[i])
        return 1;
    return 0;
}

/* Dodaje do drzewa nowy element o nazwie x i zwraca wskaźnik do tego elementu
Jeśli element o danej nazwie już znajduje się w drzewie, zwalnia pamięć po x i zwraca wskaźnik do znalezionego elementu*/
Tree insert(Tree *t, int *x) {
    Tree tmp = *t;
    if (tmp == NULL) {
        tmp = (Tree) malloc(sizeof(Node));
        if (!tmp)
            exit(1);
        tmp->name = x;
        tmp->left = NULL;
        tmp->right = NULL;
        tmp->elements = NULL;
        *t = tmp;

        return tmp;
    }
    int cmp = compareNames(x, tmp->name);
    if (cmp < 0)
        return insert(&tmp->left, x);
    else if (cmp > 0)
        return insert(&tmp->right, x);
    else {
        free(x);
        return tmp;
    }
}

/* Wypisuje infix drzewa BST (nazwy uporządkowane leksykograficznie) */
void printBST(Tree t) {
    if (t) {
        printBST(t->left);
        printArray(t->name);
        printBST(t->right);
    }
}

/* Szuka w drzewie BST elementu o nazwie x, jeśli znaleziony, zwraca do niego wskaźnik, jeśli nie zwraca NULL */
Tree find(Tree t, int *x) {
    if (t) {
        int cmp = compareNames(x, t->name);
        if (cmp < 0)
            return find(t->left, x);
        else if (cmp > 0)
            return find(t->right, x);
        else
            return t;
    }
    return NULL;
}

/* Usuwa drzewo t oraz wszystkie jego podrzewa */
void removeAll(Tree t) {
    if (t) {
        removeAll(t->left);
        removeAll(t->right);
        removeAll(t->elements);
        free(t->name);
        free(t);
    }
}

/* Usuwa element o nazwie x z drzewa BST oraz wszystkie poddrzewa tego elementu */
void removeElement(Tree *t, int *x) {
    Tree tmp = *t;
    Tree minTree;
    if (tmp) {
        int cmp = compareNames(x, tmp->name);
        if (cmp < 0)
            removeElement(&tmp->left, x);
        else if (cmp > 0)
            removeElement(&tmp->right, x);
        else {
            if (tmp->right == NULL) {
                *t = tmp->left;
                removeAll(tmp->elements);
                free(tmp->name);
                free(tmp);

            } else {
                minTree = findMin(&tmp->right);
                removeAll(tmp->elements);
                free(tmp->name);
                tmp->name = minTree->name;
                tmp->elements = minTree->elements;
                free(minTree);
            }
        }
    }
}