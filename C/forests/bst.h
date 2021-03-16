#ifndef BST_H
#define BST_H

struct Node;

typedef struct Node Node;

typedef struct Node *Tree;

struct Node {
    int *name;
    Tree left;
    Tree right;
    Tree elements;
};

Tree insert(Tree *t, int *x);

void printBST(Tree t);

Tree find(Tree t, int *x);

void removeAll(Tree t);

void removeElement(Tree *t, int *x);

#endif /* BST_H */
