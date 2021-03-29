/*
 * queue.h
 *
 *  Created on: Sep 29, 2018
 *      Author: ubuntu
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* standard linked list element */
struct elt {
    struct elt *next;
    int value;
};

struct queue {
    struct elt *head;  /* dequeue this next */
    struct elt *tail;  /* enqueue after this */
};

/* create a new empty queue */
struct queue *
queueCreate(void)
{
    struct queue *q;

    q = malloc(sizeof(struct queue));

    q->head = q->tail = 0;

    return q;
}

/* add a new value to back of queue */
void
enq(struct queue *q, int value)
{
    struct elt *e;

    e = malloc(sizeof(struct elt));
    assert(e);

    e->value = value;

    /* Because I will be the tail, nobody is behind me */
    e->next = 0;

    if(q->head == 0) {
        /* If the queue was empty, I become the head */
        q->head = e;
    } else {
        /* Otherwise I get in line after the old tail */
        q->tail->next = e;
    }

    /* I become the new tail */
    q->tail = e;
}

int
queueEmpty(const struct queue *q)
{
    return (q->head == 0);
}

/* remove and return value from front of queue */
int
deq(struct queue *q)
{
    int ret;
    struct elt *e;

    if(!queueEmpty(q))
    {
    ret = q->head->value;

    /* patch out first element */
    e = q->head;
    q->head = e->next;

    free(e);
    }
    return ret;
}

int peek(const struct queue *q)
{
   if(!queueEmpty(q))
	   return q->head->value;
   else
	   return 0;
}

/* print contents of queue on a single line, head first */
void
queuePrint(struct queue *q)
{
    struct elt *e;

    for(e = q->head; e != 0; e = e->next) {
        printf("%d ", e->value);
    }

    putchar('\n');
}

/* free a queue and all of its elements */
void
queueDestroy(struct queue *q)
{
    while(!queueEmpty(q)) {
        deq(q);
    }

    free(q);
}


#endif /* QUEUE_H_ */
