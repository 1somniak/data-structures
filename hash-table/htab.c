#include <err.h>
#include <string.h>
#include "htab.h"

uint32_t hash(char *key)
{
    size_t length = strlen(key);
    size_t i = 0;
    uint32_t hash = 0;
    while (i != length)
    {
        hash += key[i++];
        hash += hash << 10;
        hash ^= hash >> 6;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;
}

struct htab *htab_new()
{
    struct pair *data = malloc(4 * sizeof(struct pair));
    if (!data)
    {
        errx(1, "Not enough memory!");
    }

    for (int i = 0; i < 4; i++)
    {
        struct pair nulos = {0, NULL, NULL, NULL};
        data[i] = nulos;
    }

    struct htab *res = malloc(sizeof(struct htab));
    if (res == NULL)
    {
        errx(1, "Not enough memory!");
    }

    *res = (struct htab) {4, 0, data};
    return res;
}

void pair_free(struct pair *p)
{
    if (p->hkey == 0)
    {
        return;
    }

    if (p->next != NULL)
    {
        pair_free(p->next);
        free(p->next);
    }

    free(p->key);
}

void htab_clear(struct htab *ht)
{
    for (int i = 0; i < ht->capacity; i++)
    {
        pair_free(ht->data + i);
        ht->data[i] = (struct pair) {0, NULL, NULL, NULL};
    }
    ht->size = 0;
}

void htab_free(struct htab *ht)
{
    htab_clear(ht);
    free(ht->data);
    free(ht);
}

struct pair *htab_get(struct htab *ht, char *key)
{
    uint32_t adresse = hash(key);
    size_t index = adresse % ht->capacity;
    struct pair *current_bucket = ht->data + index;
    if (current_bucket->hkey == 0)
        return NULL;

    do
    {
        if (current_bucket->hkey == adresse)
        {
            return current_bucket;
        }
    } while (current_bucket->next != NULL);
    return NULL;
}

int htab_insert(struct htab *ht, char *key, void *value)
{
    uint32_t adresse = hash(key);
    size_t index = adresse % ht->capacity;
    struct pair *current_bucket = ht->data + index;

    struct pair *paire = ht->data + index;
    char *copy_key = calloc(0, sizeof(char) * (sizeof(key) + 1));
    if (copy_key == NULL)
    {
        errx(1, "Not enough memory!");
    }

    int i;
    for (i = 0; key[i] != 0; i++)
    {
        copy_key[i] = key[i];
    }
    copy_key[i] = 0;

    while (paire->hkey != 0)
    {
        if (strcmp(paire->key, key) == 0)
        {
            return 0;
        }
        if (paire->next == NULL)
            break;
        paire = paire->next;
    }

    if (current_bucket->hkey == 0)
    {
        ht->data[index] = (struct pair) {adresse, copy_key, value, NULL};
    }
    else
    {
        struct pair *bucket = malloc(sizeof(struct pair));
        *bucket = (struct pair) {current_bucket->hkey, current_bucket->key,
            current_bucket->value, current_bucket->next};
        current_bucket->hkey = adresse;
        current_bucket->key = copy_key;
        current_bucket->value = value;
        current_bucket->next = bucket;
    }
    ht->size++;

    if (100 * ht->size / ht->capacity >= 75)
    {
        htab_resize(ht, 2 * ht->capacity - 1);
    }
}

void htab_resize(struct htab *ht, size_t new_capa)
{
    size_t old_capa = ht->capacity;
    size_t old_size = ht->size;
    struct pair *new_data = malloc(new_capa * sizeof(struct pair));
    if (new_data == NULL)
        return;

    struct pair *old_data = ht->data;
    ht->data = new_data;
    ht->capacity = new_capa;
    ht->size = 0;

    for (size_t i = 0; i < old_capa; i++)
    {
        struct pair *current_bucket = old_data + i;
        if (current_bucket->hkey == 0)
            continue;

        do
        {
            htab_insert(ht, current_bucket->key, current_bucket->value);
            current_bucket = current_bucket->next;
        } while (current_bucket != NULL);
    }

    size_t new_size = ht->size;
    ht->data = old_data;
    ht->capacity = old_capa;
    ht->size = old_size;
    htab_clear(ht);
    ht->data = new_data;
    ht->capacity = new_capa;
    ht->size = new_size;
}

void htab_remove(struct htab *ht, char *key)
{
    // TODO
}

struct pair *get_pairs(struct htab *ht)
{
    struct pair *res = malloc(ht->size * sizeof(struct pair));
    if (res == NULL)
        return NULL;
    size_t index = 0;

    for (size_t i = 0; i < ht->capacity; i++)
    {
        struct pair *temp = ht->data + i;
        if (temp->hkey == 0)
            continue;
        while (temp != NULL)
        {
            res[index] = (struct pair) {
                temp->hkey, temp->key, temp->value, temp->next};
            index++;
            temp = temp->next;
        }
    }
    return res;
}

void display(struct htab *ht)
{
    struct pair *paire = ht->data;
    for (int i = 0; i < ht->capacity; i++)
    {
        printf("%i", i);
        paire = ht->data + i;

        if (paire->hkey == 0)
        {
            printf("\n");
            continue;
        }

        do
        {
            printf(" -> (%08x, %s, %s)", paire->hkey, paire->key,
                (char *)paire->value);
            paire = paire->next;
        } while (paire != NULL);
        printf("\n");
    }
}
