#include <dllist.h>

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

void init_dllist_size(dllist *list, uint64_t size)
{
    list->start  = NULL;
    list->end    = NULL;
    list->length = 0;
    list->size   = size;
}

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

void init_dllist_from_array_sized(dllist *list, uint64_t elem_size, void *array, uint64_t length, dllist_err *error)
{
    init_dllist(list, elem_size);

    for (uint64_t i = 0; i < length; i++) {

        void *val_ptr = (uint8_t *)array + (i * elem_size);

        push_ptr_back_dllist(list, val_ptr, error);

        if (is_err_dllist(*error)) {
            return;
        }
    }

    reset_err_dllist(*error);
}

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
