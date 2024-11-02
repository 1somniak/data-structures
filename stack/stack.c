#include "stack.h"

#include <stdlib.h>

// DO NOT USE THIS FUNCTION
static struct stack_item *item_push(struct stack_item *s, TYPE e)
{
    struct stack_item *res = malloc(sizeof(struct stack_item));
    res->data = e;
    res->next = NULL;
    if (s == NULL)
        return res;

    res->next = s;

    return res;
}

// DO NOT USE THIS FUNCTION
static struct stack_item *item_pop(struct stack_item *s)
{
    if (s == NULL)
        return NULL;

    if (s->next == NULL)
    {
        free(s);
        return NULL;
    }

    struct stack_item *res = s->next;
    free(s);

    return res;
}

// DO NOT USE THIS FUNCTION
static TYPE *item_peek(struct stack_item *s)
{
    return s->data;
}

struct stack *stack_init(void)
{
    struct stack *s = malloc(sizeof(struct stack));
    if (s == NULL)
        return NULL;

    s->head = NULL;
    s->size = 0;

    return s;
}

void stack_push(struct stack *s, TYPE e)
{
    s->head = item_push(s->head, e);
    s->size++;
}

TYPE *stack_pop(struct stack *s)
{
    if (s->size == 0)
        return NULL;

    TYPE *res = item_peek(s->head);
    s->head = item_pop(s->head);
    s->size--;
    return res;
}

TYPE *stack_peek(struct stack *s)
{
    return item_peek(s->head);
}

static void free_item(struct stack_item *l, bool free_data)
{
    if (l == NULL)
        return;
    free_item(l->next, free_data);
    if (free_data && l->data != NULL)
    {
        free(l->data);
    }
    free(l);
}

void stack_clear(struct stack *s, bool free_data)
{
    free_item(s->head, free_data);
    s->size = 0;
    s->head = NULL;
}

void stack_destroy(struct stack *s, bool free_data)
{
    stack_clear(s, free_data);
    free(s);
}
