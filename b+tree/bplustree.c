
#include "bplustree.h"

struct Node *bptree_new(size_t order)
{
    struct Node *res = (struct Node *)malloc(sizeof(struct Node));
    if (res == NULL)
        return NULL;

    res->order = order;
    res->nb_childrens = 0;
    res->childrens = calloc(0, sizeof(struct Node *) * (order + 1));
    res->nb_values = 0;
    res->values = malloc(sizeof(size_t) * order);
    res->next = NULL;

    return res;
}


int bptree_isvalid(struct Node *bpt)
{
    if (bpt->nb_childrens == 0)
        return 1;

    if (bpt->nb_childrens != bpt->nb_values + 1)
        return 0;

    for (size_t i = 0; i < bpt->nb_childrens; i++)
    {
        if (!bptree_isvalid(bpt->childrens + i))
            return 0;
    }

    return 1;
}

int has_childs(struct Node *bpt)
{
    return bpt->nb_childrens != 0;
}

void display(struct Node *bpt)
{
    printf("[ ");
    for (size_t i = 0; i < bpt->nb_values; i++)
    {
        printf("%zu ", bpt->values[i]);
    }
    printf(" ]\n");
}

size_t binary_search(size_t to_add, size_t *list, size_t nb_elements)
{
    size_t left = 0;
    size_t right = nb_elements;
    size_t mid = 0;
    while (left < right)
    {
        mid = left + (right - left) / 2;
        if (to_add > list[mid])
            left = mid + 1;
        else if (to_add < list[mid])
            right = mid;
        else if (to_add == list[mid])
            return mid;
    }
    return right;
}

size_t aux_insert(struct Node *bpt, size_t value)
{
    size_t index = binary_search(value, bpt->values, bpt->nb_values);

    if (!has_childs(bpt))
    {
        if (bpt->nb_values < bpt->order - 1)
        {
            for (int i = bpt->nb_values; i > index; i--)
            {
                bpt->values[i] = bpt->values[i - 1];
            }
            bpt->values[index] = value;
            bpt->nb_values++;
        }
    }
    else
    {
    }
}


void bptree_insert(struct Node *bpt, size_t value)
{
    size_t middle = aux_insert(bpt, value);

    if (bpt->nb_childrens == bpt->order + 1)
    {
        struct Node *new_left = bptree_new(bpt->order);
        struct Node *new_right = bptree_new(bpt->order);
        for (size_t i = 0; i < (bpt->order - 1) / 2; i++)
            new_left->values[i] = bpt->values[i];
        new_left->nb_values = (bpt->order - 1) / 2;
        size_t init = (bpt->order - 1) / 2 + 1;
        for (size_t i = init; i < bpt->order - 1; i++)
        {

        }
    }

}
