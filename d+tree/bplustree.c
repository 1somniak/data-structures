
#include "bplustree.h"

struct Node *bptree_new(size_t order)
{
    struct Node *res = (struct Node *)malloc(sizeof(struct Node));
    if (res == NULL)
        return NULL;

    res->order = order;
    res->nb_childrens = 0;
    res->childrens = NULL;
    res->nb_values = 0;
    res->values = malloc(sizeof(size_t) * (order - 1));

    return res;
}

void allocate_childrens(struct Node *bpt)
{
    if (bpt->childrens != NULL)
        printf("memory already allocated!\n");
    else
        bpt->childrens = calloc(0, sizeof(struct Node *) * bpt->order);
}

int bptree_isvalid(struct Node *bpt)
{
    if (bpt->nb_childrens == 0)
        return TRUE;

    if (bpt->nb_childrens != bpt->nb_values + 1)
        return FALSE;

    for (size_t i = 0; i < bpt->nb_childrens; i++)
    {
        if (!bptree_isvalid(bpt->childrens[i]))
            return FALSE;
    }

    return TRUE;
}

int has_childs(struct Node *bpt)
{
    return bpt->nb_childrens != 0;
}

void display(struct Node *bpt)
{
    printf("[ ");
    for (size_t i = 0; i < bpt->nb_values; i++)
        printf("%zu ", bpt->values[i]);

    printf("] nb_values: %zu ; nb_childrens : %zu; \n", bpt->nb_values,
        bpt->nb_childrens);
    for (size_t i = 0; i < bpt->nb_childrens; i++)
    {
        printf("[ ");
        for (size_t j = 0; j < bpt->childrens[i]->nb_values; j++)
            printf("%zu ", bpt->childrens[i]->values[j]);

        printf("] -> ");
    }
}

void aux_export(FILE *dotfile, struct Node *bpt, int id)
{
    fprintf(dotfile, "    node%d [shape=box][label=\"", id);
    for (int i = 0; i < bpt->nb_values; i++)
        fprintf(dotfile, "%zu ", bpt->values[i]);

    fprintf(dotfile, "\"];\n");

    for (int i = 0; i < bpt->nb_childrens; i++)
    {
        aux_export(
            dotfile, bpt->childrens[i], (size_t)bpt->childrens[i] % 10000);
        fprintf(dotfile, "    node%d -> node%zu;\n", id,
            (size_t)bpt->childrens[i] % 10000);
    }
}

void export(struct Node *bpt, char *path)
{
    FILE *dotfile = fopen(path, "w");
    if (dotfile == NULL)
    {
        perror("Erreur d'ouverture du fichier");
        return;
    }

    fprintf(dotfile, "digraph BTree {\n");
    aux_export(dotfile, bpt, 0);
    fprintf(dotfile, "}\n");

    fclose(dotfile);
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

void array_insert_size_t(
    size_t to_add, size_t index, size_t *list, size_t nb_elements)
{
    for (size_t i = nb_elements - 1; i > index; i--)
        list[i] = list[i - 1];

    list[index] = to_add;
}

void array_insert_node(
    struct Node *to_add, size_t index, struct Node **list, size_t nb_childrens)
{
    for (size_t i = nb_childrens - 1; i > index; i--)
        list[i] = list[i - 1];

    list[index] = to_add;
}

struct Node *aux_insert(struct Node **bpt, size_t element)
{
    if ((*bpt)->nb_childrens == 0) // Cas I
        return insert_leaf(bpt, element);
    else // Cas II
    {
        size_t index
            = binary_search(element, (*bpt)->values, (*bpt)->nb_values);
        struct Node *fils_index
            = insert_leaf((*bpt)->childrens + index, element);
        if (!has_childs((*bpt)->childrens[index])) // Cas 1
        {
            // TODO
        }
        else // Cas 2
        {
            // TODO
        }
    }
}

struct Node *insert_leaf(struct Node **bpt, size_t element)
{
    size_t index = binary_search(element, (*bpt)->values, (*bpt)->nb_values);
    if (!has_childs(*bpt)) // Cas I
    {
        if ((*bpt)->nb_values < (*bpt)->order - 1) // Cas 1
        {
            (*bpt)->nb_values++;
            array_insert_size_t(
                element, index, (*bpt)->values, (*bpt)->nb_values);
            return *bpt;
        }
        else // Cas 2
        {
            return split_leaf(bpt, element, index);
        }
    }
    else // Cas II
    {
        struct Node *temp = aux_insert((*bpt)->childrens + index, element);
        if (temp->nb_values == 1) // Cas 1
        {
            // TODO
        }
        else // Cas 2
        {
            return *bpt;
        }
    }
}

struct Node *split_leaf(struct Node **bpt, size_t element, size_t index)
{
    size_t order = (*bpt)->order;
    struct Node *left = bptree_new(order);
    struct Node *right = bptree_new(order);
    size_t last_element = (*bpt)->values[order - 2];

    array_insert_size_t(element, index, (*bpt)->values, order - 1);
    size_t middle = order / 2;

    size_t i;
    for (i = 0; i < middle; i++)
        left->values[i] = (*bpt)->values[i];
    left->nb_values = middle;

    for (i = middle; i < order - 1; i++)
        right->values[i - middle] = (*bpt)->values[i];
    right->values[i - middle] = last_element;
    right->nb_values = order - middle;

    (*bpt)->nb_childrens = 2;
    (*bpt)->childrens[0] = left;
    (*bpt)->childrens[1] = right;
    (*bpt)->nb_values = 1;
    (*bpt)->values[0] = (*bpt)->values[middle];

    return *bpt;
}

struct Node *split_node(struct Node **bpt, size_t index, struct Node *sub_bpt)
{
    size_t order = (*bpt)->order;
    struct Node *left = bptree_new(order);
    struct Node *right = bptree_new(order);
    size_t last_element = (*bpt)->values[order - 2];
    struct Node *last_node = (*bpt)->childrens[order - 1];

    array_insert_size_t(sub_bpt->values[0], index, (*bpt)->values, order - 1);
    array_insert_node(
        sub_bpt->childrens[1], index + 1, (*bpt)->childrens, order);
    (*bpt)->childrens[index] = sub_bpt->childrens[0];
    

}
