#include <stdio.h>
#include "bplustree.h"


int main(int argc, char *argv[])
{
    printf("Hello, world!\n");
    struct Bplustree *bpt = bptree_new(3);

    insert(bpt, 3);
    insert(bpt, 10);
    insert(bpt, 16);
    insert(bpt, 19);
    insert(bpt, 17);
    insert(bpt, 17);
    insert(bpt, 17);
    insert(bpt, 17);

    //insert(bpt, 17);


    /* insert(bpt, 23); */
    /* insert(bpt, 27); */
    /* insert(bpt, 28); */
    /* insert(bpt, 30); */

    export(bpt, "graph.dot", argc - 2);
    /* struct Node *found = search_min(bpt, 17); */
    /* while (found != NULL) */
    /* { */
    /*     for (size_t i = 0; i < found->nb_values; i++) */
    /*         printf("%zu ", found->values[i]); */

    /*     printf(" -> "); */
    /*     found = found->next; */
    /* } */
    /* printf("NULL\n"); */
    /* print_values(bpt); */


    return 0;
}
