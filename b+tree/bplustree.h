#ifndef BPT_TREE
#define BPT_TREE
#define TRUE 1
#define FALSE 0

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

struct Bplustree
{
    size_t order;
    struct Node *root;
};

// ouioui
struct Node
{
    size_t order;

    size_t nb_childrens;
    struct Node **childrens;

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

// Allocates a node without values and without allocating memory for childrens
struct Bplustree *bptree_new(size_t order);

struct Node *node_new(size_t order);

// Allocate the mandatory memory for the childrens.
void allocate_childrens(struct Node *bpt);

// Check if b+ Tree is valid
int bptree_isvalid(struct Bplustree *bpt);

// Display Node
void display(struct Bplustree *bpt);

void print_values(struct Bplustree *bpt);

// Make a dicotomic search to find an element or to find where to place a new
// element.
size_t binary_search(size_t to_add, size_t *list, size_t nb_elements);

void array_insert_size_t(size_t to_add, size_t index, size_t *list, size_t nb_elements);
void array_insert_node(struct Node *to_add, size_t index, struct Node **list, size_t nb_childrens);

//
void export(struct Bplustree *bpt, char *path, int display_right_links);

struct Node *search_min(struct Bplustree *bpt, size_t value);

// insert an argument in a provided bplustree struct
void insert(struct Bplustree *bpt, size_t element);

size_t bpt_remove(struct Bplustree *bpt, size_t value);

#endif
