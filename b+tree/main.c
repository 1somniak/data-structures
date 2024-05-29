#include <stdio.h>
#include "bplustree.h"


int main(int argc, char *argv[])
{
    printf("hello world\n");
    struct Node *bpt = bptree_new(3);
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
        struct s_insert *test1 = aux_insert(bpt, atoi(argv[i]));

        if (test1->cas != 0)
        {
            printf("bpt");
            display(bpt);
            printf("right: ");
            display(test1->new_child);
            printf("up: %zu \n", test1->value_to_add);
        }
        else
        {
            printf("bpt: ");
            display(bpt);
        }
        printf("\n\n");
    }
    printf("\nnewvalue:\n");
    display(bpt);


    return 0;
}
