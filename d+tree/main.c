#include <stdio.h>
#include "bplustree.h"


int main(int argc, char *argv[])
{
    printf("hello world\n");
    struct Node *bpt = bptree_new(3);
    bpt->nb_values = 2;
    bpt->values[0] = 2;
    bpt->values[1] = 4;
    allocate_childrens(bpt);
    struct Node *child1 = bptree_new(3);
    struct Node *child2 = bptree_new(3);
    struct Node *child3 = bptree_new(3);
    child1->nb_values = 2;
    child1->values[0] = 0;
    child1->values[1] = 1;
    child2->nb_values = 2;
    child2->values[0] = 2;
    child2->values[1] = 3;
    child3->nb_values = 2;
    child3->values[0] = 6;
    child3->values[1] = 8;

    bpt->childrens[0] = child1;
    bpt->childrens[1] = child2;
    bpt->childrens[2] = child3;
    bpt->nb_childrens = 3;

    /* split(&bpt); */

    export(bpt, "graph.dot");


    return 0;
}
