#ifndef _BST_H
#define _BST_H

typedef struct Node_s{
    struct Node_s *parent;
    struct Node_s *left;
    struct Node_s *right;
    int seg[];
}Node_t;

#endif