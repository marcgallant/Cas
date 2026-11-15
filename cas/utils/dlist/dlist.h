#ifndef DLIST_H
#define DLIST_H

#include <stddef.h>

struct dlist_item
{
    struct dlist_item *next;
    struct dlist_item *prev;
    void *data;
};

struct dlist
{
    size_t size;
    struct dlist_item *head;
    struct dlist_item *tail;
};

struct dlist *dlist_init(void);

int dlist_push_front(struct dlist *list, void *element);

int dlist_push_back(struct dlist *list, void *element);

size_t dlist_size(const struct dlist *list);

void dlist_destroy(struct dlist *list);

#endif // DLIST_H
