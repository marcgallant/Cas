#ifndef HASH_SET_H
#define HASH_SET_H

#include <stdbool.h>
#include <stddef.h>

struct list
{
    const char *key;
    struct list *next;
};

struct hash_set
{
    struct list **data;
    size_t size;
};

struct hash_set *hash_set_init(size_t size);

bool hash_set_insert(struct hash_set *hash_set, const char *key);

void hash_set_destroy(struct hash_set *hash_set);

bool hash_set_contains(const struct hash_set *hash_set, const char *key);

bool hash_set_remove(struct hash_set *hash_set, const char *key);

#endif /* ! HASH_SET_H */
