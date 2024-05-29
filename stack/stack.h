#ifndef STACK
#define STACK

#include <stdlib.h>

struct stack {
    size_t capacity;
    size_t index;
    void** data;
};


struct stack *stack_new();

void push(struct stack *st, void *value);

void *pop(struct stack *st);

void *top(struct stack *st);

#endif
