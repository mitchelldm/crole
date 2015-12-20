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

char *translate_dllist_err(dllist_err error);

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

#define get_val_dllist_node(node) (node ? (uint8_t *)node + sizeof(dllist_node) : NULL)

typedef struct dllist {
    dllist_node *start;
    dllist_node *end;
    uint64_t length;
    uint64_t size;
} dllist;

// Intialises a dllist with elements of size size.
void init_dllist_size(dllist *list, uint64_t size);

// Intialises a dllist with elements of type type.
#define init_dllist(list, type) init_dllist_size(list, sizeof(type))

// Pushes a new node onto the end of a list, reading list.size bytes from val_ptr.
void push_ptr_back_dllist(dllist *list, void *val_ptr, dllist_err *error);

// Pushes a new node onto the end of a list, reading list.size bytes from val cast as a pointer to type.
#define push_back_dllist(list, type, val, error) push_ptr_back_dllist(list, &(type){val}, error)

// Pushes a new node onto the start of a list, reading list.size bytes from val_ptr.
void push_ptr_front_dllist(dllist *list, void *val_ptr, dllist_err *error);

// Pushes a new node onto the start of a list, reading list.size bytes from val cast as a pointer to type.
#define push_front_dllist(list, type, val, error) push_ptr_front_dllist(list, &(type){val}, error)

// Initialises a list from array, which must consist of length items of size elem_size.
void init_dllist_from_array_sized(dllist *list, uint64_t elem_size, void *array, uint64_t length, dllist_err *error);

// Initialises a list from array, which must consist of legth items of type type.
#define init_dllist_from_array(list, type, array, length, error) init_dllist_from_array_sized(list, sizeof(type), array, length, error)

// Destroys a dllist, freeing all nodes.
void destroy_dllist(dllist *list);

// Removes value from end of a dllist
void pop_back_dllist(dllist *list);

// Removes value from start of a dllist
void pop_front_dllist(dllist *list);

// Returns a pointer to the element of a dllist at pos
void *get_ptr_dllist(dllist *list, uint64_t pos, dllist_err *error);

// Copies the element of a dllist at pos to out_val
void get_dllist(dllist *list, uint64_t pos, void *out_val, dllist_err *error);

// Returns a pointer to the first element of a dllist
#define get_start_ptr_dllist(list, error) get_ptr_dllist(list, 0, error)

// Copies the first element of a dllist to out_val
#define get_start_dllist(list, out_val, error) get_dllist(list, 0, out_val, error)

// Returns a pointer to the last element of a dllist
#define get_end_ptr_dllist(list, error) get_ptr_dllist(list, (list)->length - 1, error)

// Copies the last element of a dllist to out_val
#define get_end_dllist(list, out_val, error) get_dllist(list, (list)->length - 1, out_val, error)

void insert_ptr_dllist(dllist *list, uint64_t pos, void *val, dllist_err *error);

#define insert_dllist(list, type, pos, val, err) insert_ptr_dllist(list, pos, &(type){val}, err)

void remove_dllist(dllist *list, dllist_err *error, uint64_t pos);

typedef struct dllist_iter {
    dllist_node *cur;
    dllist_node *end;
    uint64_t size;
} dllist_iter;

dllist_iter iter_dllist(dllist *list);

dllist_iter rev_iter_dllist(dllist *list);

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
