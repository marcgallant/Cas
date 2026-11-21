#include "hash_map.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hash/hash.h>

struct hash_map *hash_map_init(size_t size)
{
    if (size == 0)
        return NULL;

    struct hash_map *res = malloc(sizeof(struct hash_map));
    if (!res)
        return NULL;

    res->data = calloc(size, sizeof(struct pair_list *));
    if (!res->data)
    {
        free(res);
        return NULL;
    }

    res->size = size;

    return res;
}

bool hash_map_insert(struct hash_map *hash_map, const char *key, char *value,
                     bool *updated)
{
    if (!hash_map || !key || hash_map->size == 0)
        return false;

    size_t h = hash(key) % hash_map->size;
    struct pair_list *tmp = hash_map->data[h];
    if (updated)
        *updated = false;

    while (tmp)
    {
        if (!strcmp(key, tmp->key))
        {
            tmp->value = value;
            if (updated)
                *updated = true;
            return true;
        }
        tmp = tmp->next;
    }

    struct pair_list *elt = malloc(sizeof(struct pair_list));
    if (!elt)
        return false;

    elt->key = key;
    elt->value = value;
    elt->next = hash_map->data[h];

    hash_map->data[h] = elt;

    return true;
}

const char *hash_map_get(const struct hash_map *hash_map, const char *key)
{
    if (!hash_map || !key || hash_map->size == 0)
        return NULL;

    size_t h = hash(key) % hash_map->size;

    struct pair_list *tmp = hash_map->data[h];

    while (tmp)
    {
        if (!strcmp(key, tmp->key))
            return tmp->value;

        tmp = tmp->next;
    }

    return NULL;
}

bool hash_map_remove(struct hash_map *hash_map, const char *key)
{
    if (!hash_map || !key || hash_map->size == 0)
        return false;

    size_t h = hash(key) % hash_map->size;

    struct pair_list *prev = NULL;
    struct pair_list *tmp = hash_map->data[h];

    while (tmp)
    {
        if (!strcmp(tmp->key, key))
        {
            if (prev)
                prev->next = tmp->next;
            else
                hash_map->data[h] = tmp->next;

            free(tmp);
            return true;
        }

        prev = tmp;
        tmp = tmp->next;
    }

    return false;
}

void hash_map_destroy(struct hash_map *hash_map)
{
    if (!hash_map)
        return;

    for (size_t i = 0; i < hash_map->size; i++)
    {
        while (hash_map->data[i])
        {
            struct pair_list *tmp = hash_map->data[i]->next;

            free(hash_map->data[i]);

            hash_map->data[i] = tmp;
        }
    }

    free(hash_map->data);
    free(hash_map);
}
