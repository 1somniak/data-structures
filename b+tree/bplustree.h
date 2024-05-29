#ifndef BPT_TREE
#define BPT_TREE

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
** B+ Tree Rules  :
** 1. Every node has at most m childrens.
** 2. Every node, except for the root and the leaves, has at least ⌈m/2⌉ childrens.
** 3. The root node has at least two childrens unless it is a leaf.
** 4. All leaves appear on the same level.
** 5. A non-leaf node with k childrens contains k−1 keys.
*/

struct Node
{
    size_t order;

    size_t nb_childrens;
    struct Node *childrens;

    size_t nb_values;
    size_t *values;

    struct Node *next;
};

/*
** Cas :
** - 0 = Added successfuly, nothing to do.
** - 1 = Not added successfuly, need to do a left tree and a right tree, and
**   to insert a node up. Has no childrens.
** - 2 = Same as 1, but has childrens.
*/
struct s_insert
{
    char cas;
    struct Node *new_child;
    size_t value_to_add;
};

// Allocates a node without values.
struct Node *bptree_new(size_t order);

// Check if b+ Tree is valid
int bptree_isvalid(struct Node *bpt);

// Display Node
void display(struct Node *bpt);

struct s_insert *aux_insert(struct Node *bpt, size_t value);

// Make a dicotomic search to find an element or to find where to place a new
// element.
size_t binary_search(size_t to_add, size_t *list, size_t nb_elements);


// Inserts a value in b+ Tree
void bptree_insert(struct Node *bpt, size_t value);


#endif
