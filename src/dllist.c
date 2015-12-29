/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <crole/dllist.h>

const char *crole_translate_dllist_err(crole_dllist_err error)
{
    static const char no_err[]        = "CROLE_DLLIST_NO_ERR";
    static const char out_of_bounds[] = "CROLE_DLLIST_OUT_OF_BOUNDS";
    static const char malloc_fail[]   = "CROLE_DLLIST_MALLOC_FAIL";
    static const char unknown[]       = "CROLE_DLLIST_UNKNOWN_ERR";

    switch (error) {
        case CROLE_DLLIST_NO_ERR:
            return no_err;

        case CROLE_DLLIST_OUT_OF_BOUNDS:
            return out_of_bounds;

        case CROLE_DLLIST_MALLOC_FAIL:
            return malloc_fail;

        default:
            return unknown;
    }
}

void crole_init_dllist_size(crole_dllist *list, uint_fast16_t size)
{
    list->start  = NULL;
    list->end    = NULL;
    list->length = 0;
    list->size   = size;
}

crole_dllist_node *crole_new_dllist_node(uint_fast16_t size, void *val_ptr, crole_dllist_err *error)
{
    void *node_ptr = malloc(sizeof(crole_dllist_node) + size);
    if (!node_ptr) {
        *error = CROLE_DLLIST_MALLOC_FAIL;
        return NULL;
    }

    memcpy(crole_get_val_dllist_node(node_ptr), val_ptr, size);

    return node_ptr;
}

void crole_push_ptr_back_dllist(crole_dllist *list, void *val_ptr, crole_dllist_err *error)
{
    crole_dllist_node *node = crole_new_dllist_node(list->size, val_ptr, error);
    if (crole_is_err_dllist(*error)) {
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
}

void crole_push_ptr_front_dllist(crole_dllist *list, void *val_ptr, crole_dllist_err *error)
{
    crole_dllist_node *node = crole_new_dllist_node(list->size, val_ptr, error);
    if (crole_is_err_dllist(*error)) {
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
}

void crole_init_dllist_from_array_size(crole_dllist *list, uint_fast16_t elem_size, void *array, size_t length, crole_dllist_err *error)
{

    crole_init_dllist_size(list, elem_size);

    for (size_t i = 0; i < length; i++) {

        void *val_ptr = (uint8_t *)array + (i * elem_size);

        crole_push_ptr_back_dllist(list, val_ptr, error);

        if (crole_is_err_dllist(*error)) {
            return;
        }
    }
}

void crole_destroy_dllist(crole_dllist *list)
{
    for (crole_dllist_node *node = list->start;
        node;
        node = node->next) {
        free(node->prev);
    }

    free(list->end);
    list->start = NULL;
    list->end   = NULL;
}

crole_dllist_node *crole_get_node_dllist(crole_dllist *list, size_t pos, crole_dllist_err *error)
{
    if (pos >= list->length) {
        *error = CROLE_DLLIST_OUT_OF_BOUNDS;
        return NULL;
    }

    if (pos == list->length - 1) {
        crole_reset_dllist_err(*error);
        return list->end;
    }

    crole_dllist_node *node = list->start;

    for (size_t i = 0; i != pos; i++) {
        node = node->next;
    }

    return node;
}

void *crole_get_ptr_dllist(crole_dllist *list, size_t pos, crole_dllist_err *error)
{
    crole_dllist_node *node = crole_get_node_dllist(list, pos, error);
    if (crole_is_err_dllist(*error)) {
        return NULL;
    }

    return crole_get_val_dllist_node(node);
}

void crole_get_dllist(crole_dllist *list, size_t pos, void *out_val, crole_dllist_err *error)
{
    void *node_content = crole_get_ptr_dllist(list, pos, error);
    if (crole_is_err_dllist(*error)) {
        return;
    }

    memcpy(out_val, node_content, list->size);
}

void crole_insert_ptr_dllist(crole_dllist *list, size_t pos, void *val, crole_dllist_err *error)
{
    crole_dllist_node *orig_at_pos = crole_get_node_dllist(list, pos, error);
    if (crole_is_err_dllist(*error)) {
        return;
    }

    crole_dllist_node *new_node = crole_new_dllist_node(list->size, val, error);
    if (crole_is_err_dllist(*error)) {
        return;
    }

    crole_dllist_node *before_pos = orig_at_pos->prev;

    new_node->next = orig_at_pos;
    new_node->prev = before_pos;

    orig_at_pos->prev = new_node;
    before_pos->next = new_node;
}

void crole_pop_back_dllist(crole_dllist *list, crole_dllist_err *error)
{
    crole_dllist_node *node = list->end;

    if (node) {
        list->end = node->prev;

        if (list->end) // list might be empty now
            list->end->next = NULL;
        else
            list->start = NULL;

        list->length--;
        free(node);

    } else {
        *error = CROLE_DLLIST_POP_EMPTY;
    }
}

void crole_pop_front_dllist(crole_dllist *list, crole_dllist_err *error)
{
    crole_dllist_node *node = list->start;

    if (node) {
        list->start = node->next;
        
        if (list->start) // list might be empty now
            list->start->prev = NULL;
        else
            list->end = NULL;
        
        list->length--;
        free(node);

    } else {
        *error = CROLE_DLLIST_POP_EMPTY;
    }
}

void crole_remove_dllist(crole_dllist *list, size_t pos, void *out_val, crole_dllist_err *error)
{
    if (pos == 0)
        crole_pop_front_dllist(list, error);
    else if (pos == list->length - 1)
        crole_pop_back_dllist(list, error);
    else { 

        crole_dllist_node *node = crole_get_node_dllist(list, pos, error);
        if (crole_is_err_dllist(*error)) {
            return;
        }

        node->prev->next = node->next;
        node->next->prev = node->prev;

        list->length--;

        if (out_val) {
            memcpy(out_val, crole_get_val_dllist_node(node), list->size);
        }

        free(node);
    }
}

crole_dllist_iter crole_iter_dllist(crole_dllist *list)
{
    return (crole_dllist_iter){list->start, list->end};
}

crole_dllist_iter crole_rev_iter_dllist(crole_dllist *list)
{
    return (crole_dllist_iter){list->end, list->start};
}
