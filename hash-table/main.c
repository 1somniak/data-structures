#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htab.h"

int main()
{
    char *countries[8] = {"France", "Spain", "Jamaica", "Cuba", "Turkey",
        "Louis", "Agathe", "Maman"};
    char *cities[8] = {"Paris", "Madrid", "Kingston", "Havana", "Ankara",
        "Rodet", "Hue", "Pot"};
    struct htab *ht = htab_new();

    for (int i = 0; i < 8; i++)
    {
        printf("\nhtab_insert(ht, %s, %s)\n", countries[i], cities[i]);
        htab_insert(ht, countries[i], cities[i]);
        //  printf("size: %li\n", ht->size);
        display(ht);
    }

    // display(ht);
    // printf("\n\n");
    // htab_resize(ht, 7);
    // display(ht);
    // printf("\n\n");
    // htab_resize(ht, 3);
    // display(ht);


    // struct pair *found = htab_get(ht, "France");
    // printf("%s\n", found->value);
    // pair_free(found);

    htab_free(ht);

    return 0;
}
