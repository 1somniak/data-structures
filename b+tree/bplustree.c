
#include "bplustree.h"

struct Node *node_new(size_t order)
{
    struct Node *res = malloc(sizeof(struct Node));
    if (res == NULL)
        return NULL;

    res->order = order;
    res->nb_childrens = 0;
    res->childrens = NULL;
    res->nb_values = 0;
    res->values = malloc(sizeof(size_t) * (order - 1));

    return res;
}

struct Bplustree *bptree_new(size_t order)
{
    struct Bplustree *res = malloc(sizeof(struct Bplustree));
    if (res == NULL)
        return NULL;

    res->order = order;
    res->root = node_new(order);

    return res;
}

void allocate_childrens(struct Node *node)
{
    if (node->childrens != NULL)
        printf("memory already allocated!\n");
    else
        node->childrens = calloc(0, sizeof(struct Node *) * node->order);
}

/*int bptree_isvalid(struct Bplustree *bpt)
{
    if (bpt->root->nb_childrens == 0)
        return TRUE;

    if (bpt->root->nb_childrens != bpt->root->nb_values + 1)
        return FALSE;

    for (size_t i = 0; i < bpt->root->nb_childrens; i++)
    {
        if (!bptree_isvalid(bpt->root->childrens[i]))
            return FALSE;
    }

    return TRUE;
}*/

int has_childs(struct Node *node)
{
    return node->nb_childrens != 0;
}

void display(struct Bplustree *bpt)
{
    printf("[ ");
    for (size_t i = 0; i < bpt->root->nb_values; i++)
        printf("%zu ", bpt->root->values[i]);

    printf("] nb_values: %zu ; nb_childrens : %zu; \n", bpt->root->nb_values,
        bpt->root->nb_childrens);
    for (size_t i = 0; i < bpt->root->nb_childrens; i++)
    {
        printf("[ ");
        for (size_t j = 0; j < bpt->root->childrens[i]->nb_values; j++)
            printf("%zu ", bpt->root->childrens[i]->values[j]);

        printf("] -> ");
    }
}

void aux_export(FILE *dotfile, struct Node *bpt, int id)
{
    fprintf(dotfile, "    node%d [shape=box][label=\"", id);
    printf("    node%d [shape=box][label=\"", id);
    for (int i = 0; i < bpt->nb_values; i++)
    {
        fprintf(dotfile, "%zu", bpt->values[i]);
        printf("%zu", bpt->values[i]);
        if (i + 1 != bpt->nb_values)
        {
            fprintf(dotfile, " ");
            printf(" ");
        }
    }

    fprintf(dotfile, "\"];\n");
    printf("\"];\n");

    for (int i = 0; i < bpt->nb_childrens; i++)
    {
        aux_export(
            dotfile, bpt->childrens[i], (size_t)bpt->childrens[i] % 10000);
        fprintf(dotfile, "    node%d -> node%zu;\n", id, (size_t)bpt->childrens[i] % 10000);
        printf("    node%d -> node%zu;\n", id, (size_t)bpt->childrens[i] % 10000);
    }
}

void export(struct Bplustree *bpt, char *path)
{
    FILE *dotfile = fopen(path, "w");
    if (dotfile == NULL)
    {
        perror("Erreur d'nouverture du fichier");
        return;
    }

    fprintf(dotfile, "digraph BTree {\n");
    printf("digraph BTree {\n");
    aux_export(dotfile, bpt->root, 0);
    fprintf(dotfile, "}\n");
    printf("}\n");

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

struct Node *add_leaf(struct Node *node, size_t element)
{
    size_t index = binary_search(element, node->values, node->nb_values);
    array_insert_size_t(element, index, node->values, node->nb_values + 1);
    node->nb_values += 1;
    return NULL;
}

struct Node *add_leaf_full(struct Node *node, size_t element)
{
    size_t index = binary_search(element, node->values, node->nb_values);
    size_t last_value;
    if (index + 1 < node->order)
    {
        last_value = node->values[node->nb_values - 1];
        array_insert_size_t(element, index, node->values, node->nb_values);
    }
    else
        last_value = element;

    size_t middle = node->order / 2;

    struct Node *left = node;
    struct Node *right = node_new(node->order);
    size_t i;
    for (i = middle; i < node->nb_values; i++)
        right->values[i - middle] = node->values[i];
    right->values[i - middle] = last_value;

    right->nb_values = left->nb_values - middle + 1;
    left->nb_values = middle;

    struct Node *res = node_new(node->order);
    allocate_childrens(res);

    res->values[0] = right->values[0];
    res->nb_values = 1;
    res->childrens[0] = left;
    res->childrens[1] = right;
    res->nb_childrens = 2;

    return res;
}

struct Node *node_insert(struct Node *node, size_t element)
{
    if (node->nb_childrens == 0) // no childrens (leaf)
    {
        if (node->nb_values + 1 < node->order) // leaf is not full
            return add_leaf(node, element);
        else // leaf is full
            return add_leaf_full(node, element);
    }
    else
    {
        size_t index = binary_search(element, node->values, node->nb_values);
        struct Node *back = node_insert(node->childrens[index], element);
        if (node->nb_values + 1 < node->order) // node is not full
        {
            if (back != NULL)
            { // PENSER A GERER LE CAS INDEX = ORDER
                if (index + 1 != node->order || 1)
                {
                    array_insert_size_t(back->values[0], index, node->values, node->nb_values + 1);
                    node->childrens[index] = back->childrens[0];
                    array_insert_node(back->childrens[1], index + 1, node->childrens, node->nb_childrens + 1);
                    node->nb_values++;
                    node->nb_childrens++;
                }
                else
                {

                }
            }
            return NULL;
        }
        else // node is full wtf
        {
            if (back != NULL)
            { // PENSER A GERER LE CAS INDEX = ORDER (done!)
                size_t last_value;
                struct Node *last_child;
                if (index + 1 != node->order)
                {
                    last_value = node->values[node->nb_values - 1];
                    array_insert_size_t(back->values[0], index, node->values, node->nb_values);
                    struct Node *last_child = node->childrens[node->nb_childrens - 1];
                    node->childrens[index] = back->childrens[0];
                    array_insert_node(back->childrens[1], index + 1, node->childrens, node->nb_childrens);
                }
                else
                {
                    last_value = back->values[0];
                    node->childrens[index] = back->childrens[0];
                    last_child = back->childrens[1];
                }

                size_t middle = node->order / 2;
                struct Node *left = node;
                struct Node *right = node_new(node->order);
                allocate_childrens(right);

                size_t i;
                for (i = middle + 1; i < node->nb_values; i++)
                {
                    right->values[i - middle - 1] = node->values[i];
                    right->childrens[i - middle - 1] = node->childrens[i];
                }
                right->values[i - middle - 1] = last_value;
                right->childrens[i - middle - 1] = node->childrens[i];
                right->childrens[i - middle] = last_child;
                right->nb_values = left->nb_values - middle;
                right->nb_childrens = left->nb_childrens - middle;
                left->nb_values = middle;
                left->nb_childrens = middle + 1;

                struct Node *res = node_new(node->order);
                allocate_childrens(res);
                res->nb_values = 1;
                res->values[0] = node->values[middle];
                res->nb_childrens = 2;
                res->childrens[0] = left;
                res->childrens[1] = right;

                return res;
            }
        }
        return NULL;
    }
}

void insert(struct Bplustree *bpt, size_t element)
{
    struct Node *node = bpt->root;
    size_t index = binary_search(element, node->values, node->nb_values);

    if (node->nb_childrens == 0)
    {
        if (node->nb_values < bpt->order - 1)
        {
            array_insert_size_t(element, index, node->values, node->nb_values + 1);
            node->nb_values++;
        }
        else
        {
            size_t last_value;
            if (index + 1 != node->order)
            {
                last_value = node->values[node->nb_values - 1];
                array_insert_size_t(element, index, node->values, node->nb_values + 1);
            }
            else
                last_value = element;

            size_t middle = node->order / 2;
            struct Node *left = node;
            struct Node *right = node_new(node->order);
            allocate_childrens(right);

            size_t i;
            for (i = middle; i < left->nb_values; i++)
                right->values[i - middle] = left->values[i];
            right->values[i - middle] = last_value;
            right->nb_values = left->nb_values - middle + 1;
            left->nb_values = middle;

            struct Node *res = node_new(node->order);
            allocate_childrens(res);
            res->nb_childrens = 2;
            res->nb_values = 1;
            res->childrens[0] = left;
            res->childrens[1] = right;
            res->values[0] = right->values[0];
            bpt->root = res;

        }
        return;
    }

    struct Node *back = node_insert(node, element);
    if (node->nb_values + 2 < node->order)
    {
        if (back != NULL)
        {
            printf("\nowoow%zu\n\n",element);
            array_insert_size_t(element, index, node->values, node->nb_values + 1);
            node->nb_values++;
            node->childrens[index] = back->childrens[0];
            array_insert_node(back->childrens[1], index + 1, node->childrens, node->nb_childrens + 1);
            node->nb_childrens++;
        }
    }
    else
    {
        if (back != NULL)
        {
            bpt->root = node_new(node->order);
            allocate_childrens(bpt->root);
            bpt->root->nb_values = 1;
            bpt->root->values[0] = back->values[0];
            bpt->root->nb_childrens = 2;
            printf("omg???\n");
            if (back->childrens[0] == node) printf("omgwtf???\n");
            bpt->root->childrens[0] = back->childrens[0];
            bpt->root->childrens[1] = back->childrens[1];
        }
    }
}
