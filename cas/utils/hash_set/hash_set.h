#ifndef HASH_SET_H
#define HASH_SET_H

#include <stdbool.h>
#include <stddef.h>

struct pair_list
{
    const char *key;
    struct pair_list *next;
};

struct hash_set
{
    struct pair_list **data;
    size_t size;
};

size_t hash(const char *str);

struct hash_set *hash_set_init(size_t size);

bool hash_set_insert(struct hash_set *hash_set, const char *key);

void hash_set_destroy(struct hash_set *hash_set);

void hash_set_dump(struct hash_set *hash_set);

bool hash_set_contains(const struct hash_set *hash_set, const char *key);

bool hash_set_remove(struct hash_set *hash_set, const char *key);

#endif /* ! HASH_SET_H */
