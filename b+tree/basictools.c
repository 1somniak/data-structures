
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
    res->next = NULL;

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

void aux_export(FILE *dotfile, struct Node *bpt, int id, int display_right_links)
{
    fprintf(dotfile, "    node%d[shape=box][label=\"", id);
    printf("    node%d[shape=box][label=\"", id);
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

                dotfile, bpt->childrens[i], (size_t)bpt->childrens[i] % 10000, display_right_links);
        fprintf(dotfile, "    node%d -> node%zu;\n", id, (size_t)bpt->childrens[i] % 10000);
        printf("    node%d -> node%zu;\n", id, (size_t)bpt->childrens[i] % 10000);
    }
    if (display_right_links && bpt->next != NULL)
        fprintf(dotfile, "    node%d -> node%zu", id, (size_t)bpt->next % 10000);
}

void export(struct Bplustree *bpt, char *path, int display_right_links)
{
    FILE *dotfile = fopen(path, "w");
    if (dotfile == NULL)
    {
        perror("Erreur d'nouverture du fichier");
        return;
    }

    fprintf(dotfile, "digraph BTree {\n");
    printf("digraph BTree {\n");
    aux_export(dotfile, bpt->root, 0, display_right_links);
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

void print_values(struct Bplustree *bpt)
{
    struct Node *current = bpt->root;
    while (current->nb_childrens != 0)
        current = current->childrens[0];

    while (current != NULL)
    {
        printf("[");
        for (size_t i = 0; i < current->nb_values; i++)
        {
            printf("%zu", current->values[i]);
            if (i + 1 != current->nb_values)
                printf(" ");
        }
        printf("] -> ");
        current = current->next;
    }
    printf("NULL\n");
}
