#include <stdio.h>
#include "operations.h"


int main() //int argc, char *argv[])
{
    printf("Hello, world!\n");
    char *a = malloc(3);
    a[0] = '2';
    a[1] = '2';
    a[2] = 0;
    add_minus(a, 2);
    printf("-22 = %s\n", a);

    printf("25 + 11 = %s\n", addition("-25", "11"));
    /* printf("25 + 17 = %s\n", addition("25", "17")); */
    /* printf("25 + 1 = %s\n", addition("25", "1")); */
    /* printf("5 + 17 = %s\n", addition("5", "17")); */
    /* printf("55 + 97 = %s\n", addition("55", "97")); */
    return 0;
}
