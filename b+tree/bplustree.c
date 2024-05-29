
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
    printf("] nb_values: %zu ; nb_childrens : %zu; \n", bpt->nb_values,
        bpt->nb_childrens);
    for (size_t i = 0; i < bpt->nb_childrens; i++)
    {
        printf("[ ");
        for (size_t j = 0; j < bpt->childrens[i].nb_values; j++)
        {
            printf("%zu ", bpt->childrens[i].values[j]);
        }
        printf("] -> ");
    }
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

void array_insert_s(size_t to_add, size_t index, size_t *list, size_t nb_elements)
{
    for (size_t i = 0; i > index; i--)
        list[i] = list[i - 1];

    list[index] = to_add;
}

void array_insert_n(struct Node *to_add, size_t index, struct Node *list, size_t nb_childrens)
{
    for (size_t i = 0; i > index; i--)
        list[i] = list[i - 1];

    list[index] = *to_add;
}

struct s_insert *aux_insert(struct Node *bpt, size_t value)
{
    size_t index = binary_search(value, bpt->values, bpt->nb_values);
    struct s_insert *res = malloc(sizeof(struct s_insert));

    if (!has_childs(bpt))
    {
        if (bpt->nb_values < bpt->order - 1)
        {
            for (int i = bpt->nb_values; i > index; i--) // insert value
                bpt->values[i] = bpt->values[i - 1];

            bpt->values[index] = value;
            bpt->nb_values++;
            res->cas = 0;
            return res;
        }
        else // Le noeud est plein, on doit split.
        {
            struct Node *right = bptree_new(bpt->order);
            right->nb_childrens = 0;

            if (index < (bpt->nb_values + 1) / 2)
            {
                for (size_t i = (bpt->nb_values + 1) / 2 - 1, j = 0;
                     i < bpt->order; i++, j++)
                {
                    right->values[j] = bpt->values[i];
                }
                res->value_to_add = (bpt->nb_values + 1) / 2 - 1;
                right->nb_values = bpt->order - index - 1;

                bpt->nb_values = (bpt->nb_values + 1) / 2 + 1;

                for (size_t i = (bpt->order - 1); i > index; i--)
                    bpt->values[i] = bpt->values[i - 1];
                bpt->values[index] = value;
                res->cas = 1;
                res->new_child = right;
                return res;
                // valeur à insérer est à gauche.
            }
            else
            {
                size_t i = bpt->order / 2;
                while (i < index)
                {
                    right->values[i - (bpt->order / 2)] = bpt->values[i];
                    i++;
                }
                right->values[i - (bpt->order / 2)] = value;
                while (i < bpt->order - 1)
                {
                    right->values[i - (bpt->order / 2) + 1] = bpt->values[i];
                    i++;
                }
                bpt->nb_values = bpt->order / 2;
                right->nb_values = bpt->order - bpt->order / 2;
                res->cas = 1;
                res->new_child = right;
                res->value_to_add = right->values[0];

                return res;
                // valeur à insérer est à droite
            }
            bpt->nb_values = (bpt->nb_values + 1) / 2;
        }
    }
    else
    {
        size_t index = binary_search(value, bpt->values, bpt->nb_values);

        if (value < bpt->values[index])
        {
            struct s_insert *temp = aux_insert(bpt, value);

            if (temp->cas == 0)
                return temp;
            else // (temp->cas == 1)
            {
                if (bpt->nb_childrens <= bpt->order)
                {
                    array_insert_s(temp->new_child->values[0], index,
                                 bpt->values, bpt->nb_values);
                    bpt->nb_values++;
                    array_insert_n(temp->new_child, index + 1,
                                 bpt->childrens, bpt->nb_childrens);
                    bpt->nb_childrens++;
                }
            }
        }
        else // (value >= bpt->values[index])
        {
            // TODO
        }
    }
}


void bptree_insert(struct Node *bpt, size_t value)
{
    size_t index = binary_search(value, bpt->values, bpt->nb_values);

    if (value < bpt->values[index])
    {
        struct s_insert *temp = aux_insert(bpt, value);

        if (temp->cas == 0)
            return temp;
        else // (temp->cas == 1)
        {
            if (bpt->nb_childrens <= bpt->order)
            {
                array_insert_s(temp->new_child->values[0], index,
                               bpt->values, bpt->nb_values);
                bpt->nb_values++;
                array_insert_n(temp->new_child, index + 1,
                             bpt->childrens, bpt->nb_childrens);
                bpt->nb_childrens++;
            }
        }
    }
    else // (value >= bpt->values[index])
    {
        // TODO
    }
    /* middle = aux_insert(bpt, value); */

    /* if (bpt->nb_childrens == bpt->order + 1) */
    /* { */
    /*    struct Node *new_left = bptree_new(bpt->order); */
    /*    struct Node *new_right = bptree_new(bpt->order); */
    /*    for (size_t i = 0; i < (bpt->order - 1) / 2; i++) */
    /*        new_left->values[i] = bpt->values[i]; */
    /*    new_left->nb_values = (bpt->order - 1) / 2; */
    /*    size_t init = (bpt->order - 1) / 2 + 1; */
    /*    for (size_t i = init; i < bpt->order - 1; i++) */
    /*    { */
    /*    } */
    /* } */

}
