
#include "bplustree.h"

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

    right->next = left->next;
    left->next = right;

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
            if (back == NULL)
                return NULL;

            array_insert_size_t(
                back->values[0], index, node->values, node->nb_values + 1);
            node->childrens[index] = back->childrens[0];
            array_insert_node(back->childrens[1], index + 1, node->childrens,
                node->nb_childrens + 1);
            node->nb_values++;
            node->nb_childrens++;
        }
        else // node is full wtf
        {
            if (back == NULL)
                return NULL;

            size_t last_value;
            struct Node *last_child;
            if (index + 1 != node->order)
            {
                last_value = node->values[node->nb_values - 1];
                array_insert_size_t(
                    back->values[0], index, node->values, node->nb_values);
                struct Node *last_child
                    = node->childrens[node->nb_childrens - 1];
                node->childrens[index] = back->childrens[0];
                array_insert_node(back->childrens[1], index + 1,
                    node->childrens, node->nb_childrens);
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
            array_insert_size_t(
                element, index, node->values, node->nb_values + 1);
            node->nb_values++;
        }
        else
        {
            size_t last_value;
            if (index + 1 != node->order)
            {
                last_value = node->values[node->nb_values - 1];
                array_insert_size_t(
                    element, index, node->values, node->nb_values + 1);
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

            left->next = right;

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
        if (back == NULL)
            return;
        array_insert_size_t(element, index, node->values, node->nb_values + 1);
        node->nb_values++;
        node->childrens[index] = back->childrens[0];
        array_insert_node(back->childrens[1], index + 1, node->childrens,
            node->nb_childrens + 1);
        node->nb_childrens++;
    }
    else
    {
        if (back == NULL)
            return;
        bpt->root = node_new(node->order);
        allocate_childrens(bpt->root);
        bpt->root->nb_values = 1;
        bpt->root->values[0] = back->values[0];
        bpt->root->nb_childrens = 2;
        bpt->root->childrens[0] = back->childrens[0];
        bpt->root->childrens[1] = back->childrens[1];
    }
}
