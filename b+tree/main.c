#include <stdio.h>
#include "bplustree.h"


int main(int argc, char *argv[])
{
    printf("hello world\n");
    struct Node *bpt = bptree_new(5);
    display(bpt);

    /* size_t oui[10] = {1, 3, 6, 10, 33, 41, 65, 88, 113, 143}; */

    /* printf("{ "); */
    /* for (int i = 0; i < 10; i++){ */
    /*     printf("%zu ", oui[i]); */
    /* } */
    /* printf("}\n"); */
    /* printf("%zu\n", binary_search((size_t)atoi(argv[1]), oui, 10)); */

    for (int i = 1; i < argc; i++)
    {
        bptree_insert(bpt, atoi(argv[i]));
        display(bpt);
    }


    return 0;
}
