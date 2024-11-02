#ifndef FIFO_H
#define FIFO_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct queue_item
{
    void *data;
    struct queue_item *next;
};

struct queue
{
    // First element inserted in the queue
    struct queue_item *head;

    // Last element inserted in the queue
    struct queue_item *tail;

    // Number of elements in the queue
    size_t size;
};

struct queue *queue_init(void);

void queue_push(struct queue *queue, void *elt);

// Returns first element inserted in the queue
void *queue_head(struct queue *queue);

// Returns last element inserted in the queue
void *queue_tail(struct queue *queue);

void *queue_pop(struct queue *queue);

void queue_print(const struct queue *queue);

void queue_clear(struct queue *queue, bool free_data);

void queue_destroy(struct queue *queue, bool free_data);

#endif /* ! FIFO_H */
