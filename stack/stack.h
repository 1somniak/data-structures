#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

#define TYPE void *

struct stack_item
{
    TYPE *data;
    struct stack_item *next;
};

struct stack
{
    struct stack_item *head;
    int size;
};

struct stack *stack_init(void);

void stack_push(struct stack *s, TYPE e);

TYPE *stack_pop(struct stack *s);

TYPE *stack_peek(struct stack *s);

void stack_clear(struct stack *s, bool free_data);

void stack_destroy(struct stack *s, bool free_data);

#endif /* !STACK_H */
