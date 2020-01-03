#ifndef _RED_BLACK_BTREE_H
#define _RED_BLACK_BTREE_H

typedef enum Color_e{BLACK, RED} Color_t;

typedef struct Node_s{
    struct Node_s *parent;
    struct Node_s *left;
    struct Node_s *right;
    Color_t color;
    int key;
}Node_t;

Node_t *getGrandParent(Node_t *n);

Node_t *getSibling(Node_t *n);

Node_t *getUncle(Node_t *n);

void rotateLeft(Node_t *n);

void rotateRight(Node_t *n);

Node_t *insert(Node_t *root, Node_t *n);

void insertRecurse(Node_t *root, Node_t *n);

void insertRepairTree(Node_t *n);

void insertCase1(Node_t *n);

void insertCase2(Node_t *n);

void insertCase3(Node_t *n);

void insertCase4(Node_t *n);

void insertCase4Step2(Node_t *n);

void replaceNode(Node_t *n, Node_t *child);

void deleteOneChild(Node_t *n);

void deleteCase1(Node_t *n);

void deleteCase2(Node_t *n);

void deleteCase3(Node_t *n);

void deleteCase4(Node_t *n);

void deleteCase5(Node_t *n);

void deleteCase6(Node_t *n);

void printRBtree(Node_t *root, int depth);

Node_t *deleteNode(Node_t *n);

Node_t *findRoot(Node_t *n);

#endif