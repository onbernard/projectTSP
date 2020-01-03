#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "red_black_Btree.h"


Node_t *getParent(Node_t *n){
    if(n == NULL){
        return NULL;
    }
    else{
        return n->parent;
    }
}

Node_t *getGrandParent(Node_t *n){
    return getParent(getParent(n));
}

Node_t *getSibling(Node_t *n){
    Node_t *p = getParent(n);
    if(p == NULL){
        return NULL;
    }
    else if(n == p->left){
        return p->right;
    }
    else{
        return p->left;
    }
}

Node_t *getUncle(Node_t *n){
    return getSibling(getParent(n));
}

void rotateLeft(Node_t *n){
    Node_t *nnew = n->right;
    Node_t *p = getParent(n);
    assert(nnew != NULL);

    n->right = nnew->left;
    nnew->left = n;
    n->parent = nnew;
    if(n->right != NULL){
        n->right->parent = n;
    }
    if(p != NULL){
        if(n == p->left){
            p->left = nnew;
        }
        else if(n == p->right){
            p->right = nnew;
        }
    }
    nnew->parent = p;
}

void rotateRight(Node_t *n){
    Node_t *nnew = n->left;
    Node_t *p = getParent(n);
    assert(nnew != NULL);

    n->left = nnew->right;
    nnew->right = n;
    n->parent = nnew;
    if(n->left != NULL){
        n->left->parent = n;
    }
    if(p!= NULL){
        if(n == p->left){
            p->left = nnew;
        }
        else if(n == p->right){
            p->right = nnew;
        }
    }
    nnew->parent = p;
}

Node_t *insert(Node_t *root, Node_t *n){
    insertRecurse(root, n);

    insertRepairTree(n);

    root = n;
    while(getParent(root) != NULL){
        root = getParent(root);
    }
    return root;
}

void insertRecurse(Node_t *root, Node_t *n){
    if(root != NULL && n->key < root->key){
        if(root->left != NULL){
            insertRecurse(root->left, n);
            return;
        }
        else{
            root->left = n;
        }
    }
    else if(root != NULL){
        if(root->right != NULL){
            insertRecurse(root->right, n);
            return;
        }
        else{
            root->right = n;
        }
    }
    n->parent = root;
    n->left = NULL;
    n->right = NULL;
    n->color = RED;
}

void insertRepairTree(Node_t *n){
    if(getParent(n) == NULL){
        insertCase1(n);
    }
    else if(getParent(n)->color == BLACK){
        insertCase2(n);
    }
    else if(getUncle(n) != NULL && getUncle(n)->color == RED){
        insertCase3(n);
    }
    else{
        insertCase4(n);
    }
}

void insertCase1(Node_t *n){
    if(getParent(n) == NULL){
        n->color = BLACK;
    }
}

void insertCase2(Node_t *n){
    return;
}

void insertCase3(Node_t *n){
    getParent(n)->color = BLACK;
    getUncle(n)->color = BLACK;
    getGrandParent(n)->color = RED;
    insertRepairTree(getGrandParent(n));
}

void insertCase4(Node_t *n){
    Node_t *p = getParent(n);
    Node_t *g = getGrandParent(n);

    if(n == p->right && p == g->left){
        rotateLeft(p);
        n = n->left;
    }
    else if(n == p->left && p == g->right){
        rotateRight(p);
        n = n->right;
    }
    insertCase4Step2(n);
}

void insertCase4Step2(Node_t *n){
    Node_t *p = getParent(n);
    Node_t *g = getGrandParent(n);

    if(n == p->left){
        rotateRight(g);
    }
    else{
        rotateLeft(g);
    }
    p->color = BLACK;
    g->color = RED;
}

void replaceNode(Node_t *n, Node_t *child){
    child->parent = n->parent;
    if(n->parent == NULL){
        return;
    }
    if(n == n->parent->left){
        n->parent->left = child;
    }
    else{
        n->parent->right = child;
    }
}

void deleteOneChild(Node_t *n){
    Node_t *child = (n->right == NULL) ? n->left : n->right;
    assert(child);
    replaceNode(n, child);
    if( (n->color) == BLACK){
        if( (child->color) == RED){
            child->color = BLACK;
        }
        else{
            deleteCase1(child);
        }
    }
    //free(n);
}

Node_t *findRoot(Node_t *n){
    assert(n != NULL);
    while(n->parent != NULL){
        n = n->parent;
    }
    return n;
}

Node_t *deleteNode(Node_t *n){
    assert(n != NULL);
    Node_t *root = findRoot(n);
    if(n->left == NULL && n->right == NULL){
        if(n->parent != NULL && n == n->parent->left){
            n->parent->left = NULL;
            
        }
        else if(n->parent != NULL && n == n->parent->right){
            n->parent->right = NULL;
        }
    }
    else{
        deleteOneChild(n);
    }
    return root;
}

void deleteCase1(Node_t *n){
    printf("deletecase1\n");
    if(n->parent != NULL){
        deleteCase2(n);
    }
}

void deleteCase2(Node_t *n){
    printf("deletecase2\n");
    Node_t *s = getSibling(n);

    if(s->color == RED){
        n->parent->color = RED;
        s->color = BLACK;
        if(n == n->parent->left){
            rotateLeft(n->parent);
        }
        else{
            rotateRight(n->parent);
        }
    }
    deleteCase3(n);
}

void deleteCase3(Node_t *n){
    printf("deletecase3\n");
    Node_t *s = getSibling(n);
    if( (n->parent->color == BLACK) && (s->color == BLACK) && (s->left->color == BLACK) && (s->right->color == BLACK) ){
        s->color = RED;
        deleteCase1(n->parent);
    }
    else{
        deleteCase4(n);
    }
}

void deleteCase4(Node_t *n){
    printf("deletecase4\n");
    Node_t *s = getSibling(n);

    if( (n->parent->color == RED) && (s->color == BLACK) && (s->left->color == BLACK) && (s->right->color == BLACK) ){
        s->color = RED;
        n->parent->color = BLACK;
    }
    else{
        deleteCase5(n);
    }
}

void deleteCase5(Node_t *n){
    printf("deletecase5\n");
    Node_t *s = getSibling(n);

    if(s->color == BLACK){
        if( (n == n->parent->left) && (s->right->color == BLACK) && s->left->color == RED ){
            s->color = RED;
            s->left->color = BLACK;
            rotateRight(s);
        }
        else if( (n == n->parent->right) && (s->left->color == BLACK) && (s->right->color == RED) ){
            s->color = RED;
            s->right->color = BLACK;
            rotateLeft(s);
        }
    }
}

void deleteCase6(Node_t *n){
    printf("deletecase6\n");
    Node_t *s = getSibling(n);

    s->color = n->parent->color;
    n->parent->color = BLACK;

    if( n == n->parent->left ){
        s->right->color = BLACK;
        rotateLeft(n->parent);
    }
    else{
        s->left->color = BLACK;
        rotateRight(n->parent);
    }
}

void printRBtree(Node_t *root, int depth){
    for(int i=0; i<depth; i++){
        printf("     ");
    }
    if(root == NULL){
        printf("NIL\n");
        return;
    }
    if(root->color == BLACK){
        printf("BLACK  ");
    }
    else{
        printf("RED  ");
    }
    printf("key = %d\n", root->key);
    printRBtree(root->left, depth+1);
    printRBtree(root->right, depth+1);
    return;
}

