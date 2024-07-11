#include "bplustree.h"


struct Node *search_min(struct Bplustree *bpt, size_t value)
{
    struct Node *current = bpt->root;

    if (current->nb_values == 0)
        return NULL;

    size_t index;
    while (current->nb_childrens != 0)
    {
        index = binary_search(value, current->values, current->nb_values);
        current = current->childrens[index];
    }

    index = binary_search(value, current->values, current->nb_values);
    if (index == current->nb_values)
        return current->next;
    else
        return current;
}
