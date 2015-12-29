/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CROLE_DLLIST_H
#define CROLE_DLLIST_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef enum {
    CROLE_DLLIST_NO_ERR         = 0,
    CROLE_DLLIST_OUT_OF_BOUNDS  = -1,
    CROLE_DLLIST_MALLOC_FAIL    = -2,
    CROLE_DLLIST_POP_EMPTY      = -3
} crole_dllist_err;

const char *crole_translate_dllist_err(crole_dllist_err error);

#define crole_is_err_dllist(error) ((error) != CROLE_DLLIST_NO_ERR)

#define crole_reset_dllist_err(error) ((error) = CROLE_DLLIST_NO_ERR)

// The value contained in each node is allocated immediately after the node itself.
// This is done for efficiency, to avoid having to do two malloc/free calls rather than just one.
// To access a pointer to a node's value, use the macro crole_get_val_dllist_node(node).
typedef struct crole_dllist_node {
    struct crole_dllist_node *next;
    struct crole_dllist_node *prev;
    // T node_contents
} crole_dllist_node;

#define crole_get_val_dllist_node(node) ((uint8_t *)node + sizeof(crole_dllist_node))

typedef struct crole_dllist {
    crole_dllist_node *start;
    crole_dllist_node *end;
    size_t length;
    uint_fast16_t size;
} crole_dllist;

// Intialises a dllist with elements of size size.
void crole_init_dllist_size(crole_dllist *list, uint_fast16_t size);

// Intialises a dllist with elements of type type.
#define crole_init_dllist(list, type) crole_init_dllist_size(list, sizeof(type))

// Pushes a new node onto the end of a list, reading list.size bytes from val_ptr.
void crole_push_ptr_back_dllist(crole_dllist *list, void *val_ptr, crole_dllist_err *error);

// Pushes a new node onto the end of a list, reading list.size bytes from val cast as a pointer to type.
#define crole_push_back_dllist(list, type, val, error) crole_push_ptr_back_dllist(list, &(type){val}, error)

// Pushes a new node onto the start of a list, reading list.size bytes from val_ptr.
void crole_push_ptr_front_dllist(crole_dllist *list, void *val_ptr, crole_dllist_err *error);

// Pushes a new node onto the start of a list, reading list.size bytes from val cast as a pointer to type.
#define crole_push_front_dllist(list, type, val, error) crole_push_ptr_front_dllist(list, &(type){val}, error)

// Initialises a list from array, which must consist of length items of size elem_size.
void crole_init_dllist_from_array_size(crole_dllist *list, uint_fast16_t elem_size, void *array, size_t length, crole_dllist_err *error);

// Initialises a list from array, which must consist of legth items of type type.
#define crole_init_dllist_from_array(list, type, array, length, error) crole_init_dllist_from_array_sized(list, sizeof(type), array, length, error)

// Destroys a dllist, freeing all nodes.
void crole_destroy_dllist(crole_dllist *list);

// Removes value from end of a dllist
void crole_pop_back_dllist(crole_dllist *list, crole_dllist_err *error);

// Removes value from start of a dllist
void crole_pop_front_dllist(crole_dllist *list, crole_dllist_err *error);

// Returns a pointer to the element of a dllist at pos
void *crole_get_ptr_dllist(crole_dllist *list, size_t pos, crole_dllist_err *error);

// Copies the element of a dllist at pos to out_val
void crole_get_dllist(crole_dllist *list, size_t pos, void *out_val, crole_dllist_err *error);

// Returns a pointer to the first element of a dllist
#define crole_get_start_ptr_dllist(list, error) crole_get_ptr_dllist(list, 0, error)

// Copies the first element of a dllist to out_val
#define crole_get_start_dllist(list, out_val, error) crole_get_dllist(list, 0, out_val, error)

// Returns a pointer to the last element of a dllist
#define crole_get_end_ptr_dllist(list, error) crole_get_ptr_dllist(list, (list)->length - 1, error)

// Copies the last element of a dllist to out_val
#define crole_get_end_dllist(list, out_val, error) crole_get_dllist(list, (list)->length - 1, out_val, error)

// Inserts value at pointer into dllist at pos, moving all elements after to the next position
void crole_insert_ptr_dllist(crole_dllist *list, size_t pos, void *val, crole_dllist_err *error);

// Inserts value into dllist at pos, moving all elements after to the next position
#define crole_insert_dllist(list, type, pos, val, err) crole_insert_ptr_dllist(list, pos, &(type){val}, err)

// Remove value from dllist at pos, optionally copying it to out_val if it is not NULL
void crole_remove_dllist(crole_dllist *list, size_t pos, void *out_val, crole_dllist_err *error);

typedef struct crole_dllist_iter {
    crole_dllist_node *cur;
    crole_dllist_node *end;
} crole_dllist_iter;

// Get a forward iterator of dllist
crole_dllist_iter crole_iter_dllist(crole_dllist *list);

// Get a reverse iterator of dllist
crole_dllist_iter crole_rev_iter_dllist(crole_dllist *list);

// Get the next node in a forward dllist iterator
#define crole_next_iter_dllist(iter) ((iter).cur = (iter).cur->next)

// Get the next node in a reverse dllist iterator
#define crole_next_rev_iter_dllist(iter) ((iter).cur = (iter).cur->prev)

// Checks if a forward iterator is finished
#define crole_more_iter_dllist(iter) ((iter).cur)

// Returns the value at the position in the dllist of the dllist iterator
#define crole_get_iter_dllist(iter, type) ((type *)crole_get_val_dllist_node((iter).cur))

// A block that is excuted once for each node in a dllist,
// with var assigned to a pointer to the current node's value
// Note that a variable named "var_iter" will be created outsize the for loop
#define crole_foreach_dllist(list, type, var) \
    crole_dllist_iter var##_iter = crole_iter_dllist(list); \
    for (type *var = crole_get_iter_dllist(var##_iter, type); crole_more_iter_dllist(var##_iter); \
        crole_next_iter_dllist(var##_iter), var = crole_get_iter_dllist(var##_iter, type))

// A block that is excuted once for each node in a dllist in reverse,
// with var assigned to a pointer to the current node's value
// Note that a variable named "var_iter" will be created outsize the for loop
#define crole_foreach_reverse_dllist(list, type, var) \
    crole_dllist_iter var##_iter = crole_rev_iter_dllist(list); \
    for (type *var = crole_get_iter_dllist(var##_iter, type); crole_more_iter_dllist(var##_iter); \
        crole_next_rev_iter_dllist(var##_iter), var = crole_get_iter_dllist(var##_iter, type))

#endif
