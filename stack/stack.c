
#include "stack.h"
#define INITIAL_SIZE 4

struct stack *stack_new()
{
    struct stack *st = malloc(sizeof(struct stack) * INITIAL_SIZE);

    st->index = 0;
    st->capacity = INITIAL_SIZE;
    st->data = malloc(sizeof(void *));

    return st;
}

void expand(struct stack *st, size_t new_size)
{
    st->capacity = new_size;
}

void push(struct stack *st, void *value)
{
    // TODO
}

