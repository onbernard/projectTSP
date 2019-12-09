#include "basic_Q.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"

static node_t *head = NULL;
static node_t *tail = NULL;

_Bool is_empty_Q(){
    return (head == NULL);
}

int push_Q(void *data){
    if(head == NULL){ // INIT QUEUE
        if( (head = (node_t *) malloc(sizeof(node_t)) ) == NULL ){
            error();
            return -1;
        }
        tail = head;
        head->next = NULL;
    }
    else{
        if( (tail->next = (node_t *) malloc(sizeof(node_t))) == NULL ){
            error();
            return -1;
        }
        tail = tail->next;
    }
    tail->data = data;
    return 0;
}


void *pop_Q(){
    if( is_empty_Q() ){
        return NULL;
    }
    void *data = head->data;
    node_t *newHead = head->next;
    free(head);
    head = newHead;
    return data;
}