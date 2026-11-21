#include "hash_set.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hash/hash.h>

struct hash_set *hash_set_init(size_t size)
{
    if (size == 0)
        return NULL;

    struct hash_set *res = malloc(sizeof(struct hash_set));
    if (!res)
        return NULL;

    res->data = calloc(size, sizeof(struct list *));
    if (!res->data)
    {
        free(res);
        return NULL;
    }

    res->size = size;

    return res;
}

bool hash_set_insert(struct hash_set *hash_set, const char *key)
{
    if (!hash_set || !key || hash_set->size == 0)
        return false;

    size_t h = hash(key) % hash_set->size;
    struct list *tmp = hash_set->data[h];

    while (tmp)
    {
        if (!strcmp(key, tmp->key))
            return false;

        tmp = tmp->next;
    }

    struct list *elt = malloc(sizeof(struct list));
    if (!elt)
        return false;

    elt->key = key;
    elt->next = hash_set->data[h];

    hash_set->data[h] = elt;

    return true;
}

bool hash_set_contains(const struct hash_set *hash_set, const char *key)
{
    if (!hash_set || !key || hash_set->size == 0)
        return false;

    size_t h = hash(key) % hash_set->size;

    struct list *tmp = hash_set->data[h];

    while (tmp)
    {
        if (!strcmp(key, tmp->key))
            return true;

        tmp = tmp->next;
    }

    return false;
}

bool hash_set_remove(struct hash_set *hash_set, const char *key)
{
    if (!hash_set || !key || hash_set->size == 0)
        return false;

    size_t h = hash(key) % hash_set->size;

    struct list *prev = NULL;
    struct list *tmp = hash_set->data[h];

    while (tmp)
    {
        if (!strcmp(tmp->key, key))
        {
            if (prev)
                prev->next = tmp->next;
            else
                hash_set->data[h] = tmp->next;

            free(tmp);
            return true;
        }

        prev = tmp;
        tmp = tmp->next;
    }

    return false;
}

void hash_set_destroy(struct hash_set *hash_set)
{
    if (!hash_set)
        return;

    for (size_t i = 0; i < hash_set->size; i++)
    {
        while (hash_set->data[i])
        {
            struct list *tmp = hash_set->data[i]->next;

            free(hash_set->data[i]);

            hash_set->data[i] = tmp;
        }
    }

    free(hash_set->data);
    free(hash_set);
}
