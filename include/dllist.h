#ifndef DLLIST_H
#define DLLIST_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef enum {
    DLLIST_NO_ERR               = 0,
    DLLIST_OUT_OF_BOUNDS        = -1,
    DLLIST_MALLOC_FAIL          = -2
} dllist_err;

char *translate_dllist_err(dllist_err error)
{
    switch (error) {
        case DLLIST_NO_ERR:
            return "DLLIST_NO_ERR";

        case DLLIST_OUT_OF_BOUNDS:
            return "DLLIST_OUT_OF_BOUNDS";

        case DLLIST_MALLOC_FAIL:
            return "DLLIST_MALLOC_FAIL";

        default:
            return "DLLIST_UNKNOWN_ERR";
    }
}

#define is_err_dllist(error) ((error) != DLLIST_NO_ERR)

#define reset_err_dllist(error) ((error) = DLLIST_NO_ERR)

// The value contained in each node is allocated immediately after the node itself.
// This is done for efficiency, to avoid having to do two malloc/free calls rather than just one.
// To access a pointer to a node's value, use the macro get_val_dllist_node(node).
typedef struct dllist_node {
    struct dllist_node *next;
    struct dllist_node *prev;
    // T node_contents
} dllist_node;

#define get_val_dllist_node(node) (node ? (void *)node + sizeof(dllist_node) : NULL)

typedef struct dllist {
    dllist_node *start;
    dllist_node *end;
    uint64_t length;
    uint64_t size;
} dllist;

// Intialises a dllist with elements of size size.
void init_dllist_size(dllist *list, uint64_t size)
{
    list->start  = NULL;
    list->end    = NULL;
    list->length = 0;
    list->size   = size;
}

// Intialises a dllist with elements of type type.
#define init_dllist(list, type) init_dllist_size(list, sizeof(type))

// Internal function, do not use.
dllist_node *__new_dllist_node(uint64_t size, void *val_ptr, dllist_err *error)
{
    void *node_ptr = malloc(sizeof(dllist_node) + size);
    if (!node_ptr) {
        *error = DLLIST_MALLOC_FAIL;
        return NULL;
    }

    void *node_content = get_val_dllist_node(node_ptr);
    memcpy(node_content, val_ptr, size);

    reset_err_dllist(*error);
    return node_ptr;
}

// Pushes a new node onto the end of a list, reading list.size bytes from val_ptr.
void push_ptr_back_dllist(dllist *list, void *val_ptr, dllist_err *error)
{
    dllist_node *node = __new_dllist_node(list->size, val_ptr, error);
    if (is_err_dllist(*error)) {
        return;
    }
    
    node->prev = list->end;         // Even if list is empty, making list->end NULL, this is still valid,
                                    // as the first element in a list's prev is NULL.
    node->next = NULL;

    if (list->end) {                // If the list wasn't already empty
        list->end->next = node;     // extend the list to the new node.
    } else {
        list->start = node;         // Otherwise make the start of the list the new node,
    }                               // in addition to being the end of the list.

    list->end = node;
    list->length++;

    reset_err_dllist(*error);
}

// Pushes a new node onto the end of a list, reading list.size bytes from val cast as a pointer to type.
#define push_back_dllist(list, type, val, error) push_ptr_back_dllist(list, &(type){val}, error)

// Pushes a new node onto the start of a list, reading list.size bytes from val_ptr.
void push_ptr_front_dllist(dllist *list, void *val_ptr, dllist_err *error)
{
    dllist_node *node = __new_dllist_node(list->size, val_ptr, error);
    if (is_err_dllist(*error)) {
        return;
    }

    node->next = list->start;       // Even if list is empty, making list->start NULL, this is still valid,
                                    // as the first element in a list's next is NULL.
    node->prev = NULL;

    if (list->start) {              // If the list wasn't already empty
        list->start->prev = node;   // extend the list to the new node.
    } else {
        list->end = node;           // Otherwise make the start of the list the new node
    }                               // in addition to being the end of the list.

    list->start = node;
    list->length++;

    reset_err_dllist(*error);
}

// Pushes a new node onto the start of a list, reading list.size bytes from val cast as a pointer to type.
#define push_front_dllist(list, type, val, error) push_ptr_front_dllist(list, &(type){val}, error)

// Initialises a list from array, which must consist of length items of size elem_size.
void init_dllist_from_array_sized(dllist *list, uint64_t elem_size, void *array, uint64_t length, dllist_err *error)
{
    init_dllist(list, elem_size);

    for (uint64_t i = 0; i < length; i++) {

        void *val_ptr = array + (i * elem_size);

        push_ptr_back_dllist(list, val_ptr, error);

        if (is_err_dllist(*error)) {
            return;
        }
    }

    reset_err_dllist(*error);
}

// Initialises a list from array, which must consist of legth items of type type.
#define init_dllist_from_array(list, type, array, length, error) init_dllist_from_array_sized(list, sizeof(type), array, length, error)

// Destroys a dllist, freeing all nodes.
void destroy_dllist(dllist *list)
{
    for (dllist_node *node = list->start;
        node;
        node = node->next) {
        free(node->prev);
    }

    free(list->end);
    list->start = NULL;
    list->end   = NULL;
}

void pop_back_dllist(dllist *list)
{
    dllist_node *node = list->end;

    if (node) {
        list->end = node->prev;

        if (list->end)
            list->end->next = NULL;
    }

    list->length--;

    free(node);
}

void pop_front_dllist(dllist *list)
{
    dllist_node *node = list->start;

    if (node) {
        list->start = node->next;

        if (list->start)
            list->start->prev = NULL;
    }

    list->length--;

    free(node);
}


dllist_node *__get_node_dllist(dllist *list, uint64_t pos, dllist_err *error)
{
    if (pos >= list->length) {
        *error = DLLIST_OUT_OF_BOUNDS;
        return NULL;
    }

    if (pos == list->length - 1) {
        reset_err_dllist(*error);
        return list->end;
    }

    dllist_node *node = list->start;
    
    for (uint64_t i = 0; i != pos; i++) {
        node = node->next;
    }

    reset_err_dllist(*error);
    return node;
}

void *get_ptr_dllist(dllist *list, uint64_t pos, dllist_err *error)
{
    dllist_node *node = __get_node_dllist(list, pos, error);
    if (is_err_dllist(*error)) {
        return NULL;
    }

    reset_err_dllist(*error);
    return get_val_dllist_node(node);
}

void get_dllist(dllist *list, uint64_t pos, void *out_val, dllist_err *error)
{
    void *node_content = get_ptr_dllist(list, pos, error);
    if (is_err_dllist(*error)) {
        return;
    }

    memcpy(out_val, node_content, list->size);

    reset_err_dllist(*error);
}

#define get_start_dllist(list, out_val, error) get_dllist(list, 0, out_val, error)

#define get_end_dllist(list, out_val, error) get_dllist(list, (list)->length - 1, out_val, error)

void insert_ptr_dllist(dllist *list, uint64_t pos, void *val, dllist_err *error)
{
    dllist_node *orig_at_pos = __get_node_dllist(list, pos, error);
    if (is_err_dllist(*error)) {
        return;
    }

    dllist_node *new_node = __new_dllist_node(list->size, val, error);
    if (is_err_dllist(*error)) {
        return;
    }

    dllist_node *before_pos = orig_at_pos->prev;

    new_node->next = orig_at_pos;
    new_node->prev = before_pos;

    orig_at_pos->prev = new_node;
    before_pos->next = new_node;
    
    reset_err_dllist(*error);
}

#define insert_dllist(list, type, pos, val, err) insert_ptr_dllist(list, pos, &(type){val}, err)

void remove_dllist(dllist *list, dllist_err *error, uint64_t pos)
{
    dllist_node *node = __get_node_dllist(list, pos, error);

    if (is_err_dllist(*error)) {
        return;
    }

    node->prev->next = node->next;

    free(node);

    reset_err_dllist(*error);
}

typedef struct dllist_iter {
    dllist_node *cur;
    dllist_node *end;
    uint64_t size;
} dllist_iter;

dllist_iter iter_dllist(dllist *list)
{
    dllist_iter iter = {list->start, list->end, list->size};
    return iter;
}

dllist_iter rev_iter_dllist(dllist *list)
{
    dllist_iter iter = {list->end, list->start, list->size};
    return iter;
}

#define next_iter_dllist(iter) ((iter).cur = (iter).cur->next)

#define next_rev_iter_dllist(iter) ((iter).cur = (iter).cur->prev)

#define more_iter_dllist(iter) ((iter).cur != (iter).end->next)

#define more_rev_iter_dllist(iter) ((iter).cur != (iter).end->prev)

#define get_iter_dllist(iter, type) ((type *)(get_val_dllist_node((iter).cur)))

#define foreach_dllist(list, type, var) \
        dllist_iter var##_iter = iter_dllist(list); \
        for (type *var = get_iter_dllist(var##_iter, type); more_iter_dllist(var##_iter); \
            next_iter_dllist(var##_iter), var##_iter.cur ? var = get_iter_dllist(var##_iter, type) : 0)

#define foreach_reverse_dllist(list, type, var) \
        dllist_iter var##_iter = rev_iter_dllist(list); \
        for (type *var = get_iter_dllist(var##_iter, type); more_rev_iter_dllist(var##_iter); \
            next_rev_iter_dllist(var##_iter), var##_iter.cur ? var = get_iter_dllist(var##_iter, type) : 0)
#endif
