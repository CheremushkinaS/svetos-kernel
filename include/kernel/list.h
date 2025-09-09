#ifndef _KERNEL_LIST_H
#define _KERNEL_LIST_H

#include <stddef.h>

typedef struct list_node {
    struct list_node* prev;
    struct list_node* next;
} list_node_t;

static inline void list_init(list_node_t* node) {
    node->prev = node;
    node->next = node;
}

static inline void list_add_tail(list_node_t* head, list_node_t* new) {
    new->prev = head->prev;
    new->next = head;
    head->prev->next = new;
    head->prev = new;
}

static inline void list_del(list_node_t* entry) {
    entry->prev->next = entry->next;
    entry->next->prev = entry->prev;
    entry->next = NULL;
    entry->prev = NULL;
}

#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_entry(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))

#endif
