#ifndef _basic_Q_H
#define _basic_Q_H

#include <stdlib.h>

typedef struct node_s{
    void *data;
    void *next;
} node_t;

_Bool is_empty_Q();

int push_Q(void *data);

void *pop_Q();

#endif