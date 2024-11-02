#include "queue.h"

struct queue *queue_init(void)
{
    struct queue *res = malloc(sizeof(struct queue));
    if (res == NULL)
        return NULL;
    res->head = NULL;
    res->tail = NULL;
    res->size = 0;

    return res;
}

void queue_push(struct queue *queue, void *elt)
{
    struct queue_item *l = malloc(sizeof(struct queue_item));
    l->data = elt;
    l->next = NULL;
    if (queue->head == NULL)
    {
        queue->head = l;
        queue->tail = l;
        queue->size = 1;
    }
    else
    {
        queue->tail->next = l;
        queue->tail = l;
        queue->size++;
    }
    return;
}

void *queue_head(struct queue *queue)
{
    return queue->head->data;
}

void *queue_tail(struct queue *queue)
{
    return queue->tail->data;
}

void *queue_pop(struct queue *queue)
{
    if (queue->head == NULL)
        return NULL;
    void *res = queue_head(queue);
    if (queue->head->next == NULL)
    {
        free(queue->head);
        queue->size = 0;
        queue->head = NULL;
        return res;
    }
    struct queue_item *to_free = queue->head;
    queue->head = queue->head->next;
    free(to_free);
    queue->size--;
    return res;
}

void queue_print(const struct queue *queue)
{
    struct queue_item *l = queue->head;
    while (l != NULL)
    {
        printf("data: %p\n", l->data);
        l = l->next;
    }
}

static void free_list(struct queue_item *l, bool free_data)
{
    if (l == NULL)
        return;
    free_list(l->next, free_data);
    if (free_data && l->data != NULL)
    {
        free(l->data);
    }
    free(l);
}

void queue_clear(struct queue *queue, bool free_data)
{
    free_list(queue->head, free_data);
    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;
}

void queue_destroy(struct queue *queue, bool free_data)
{
    queue_clear(queue, free_data);
    free(queue);
}
