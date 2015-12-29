/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CROLE_VEC_H
#define CROLE_VEC_H

#include <stdlib.h>
#include <stdio.h>

typedef enum {
    CROLE_VEC_NO_ERR = 0,
    CROLE_VEC_OUT_OF_BOUNDS = -1,
    CROLE_VEC_MALLOC_FAIL = -2,
    CROLE_VEC_POP_EMPTY = -3
} crole_vec_err;

const char *crole_translate_vec_err(crole_vec_err error);

char crole_set_vec_err(crole_vec_err *error, crole_vec_err err_type);

#define crole_is_err_vec(error) ((error) != CROLE_VEC_NO_ERR)

#define crole_reset_vec_err(error) ((error) = CROLE_VEC_NO_ERR)

#define crole_declare_vec(type) \
    typedef struct { \
        type *array; \
        size_t allocated; \
        size_t length; \
    } crole_vec_##type

#define crole_vec(type) struct crole_vec_##type

#define crole_resize_array_vec(vec, type, error) do { \
        void *new_array = realloc((vec).array, sizeof(type) * (vec).allocated); \
        if (new_array) \
            (vec).array = new_array; \
        else error = CROLE_VEC_MALLOC_FAIL; \
    } while (0)

#define crole_init_vec(vec, type, error) do { \
        (vec).allocated = 2; \
        (vec).length = 0; \
        (vec).array = malloc(sizeof(type) * (vec).allocated); \
        if (!(vec).array) \
            error = CROLE_VEC_MALLOC_FAIL; \
    } while (0)

#define crole_in_bounds_vec(vec, pos) ((size_t)(pos) < (vec).length)

#define crole_append_vec(vec, type, value, error) do { \
        if ((vec).length == (vec).allocated) { \
            (vec).allocated *= 2; \
            crole_resize_array_vec(vec, type, error); \
        } \
        (vec).array[(vec).length++] = value; \
    } while (0)

#define crole_get_unchecked_vec(vec, pos) ((vec).array[pos])

#define crole_get_vec(vec, type, pos, error) \
    (crole_in_bounds_vec(vec, pos) ? \
        crole_get_unchecked_vec(vec, pos) \
    : \
        (type)crole_set_vec_err(&error, CROLE_VEC_OUT_OF_BOUNDS))

#define crole_get_ptr_unchecked_vec(vec, pos) ((vec).array + pos)

#define crole_get_ptr_vec(vec, type, pos, error) \
    (crole_in_bounds_vec(vec, pos) ? \
        crole_get_ptr_unchecked_vec(vec, pos) \
    : \
        (type)crole_set_vec_err(&error, CROLE_VEC_OUT_OF_BOUNDS))

#define crole_set_unchecked_vec(vec, pos, value) do { \
        (vec).array[pos] = (value); \
    } while (0)

#define crole_set_vec(vec, type, pos, value, error) do { \
        if (crole_in_bounds_vec(vec, pos)) { \
            crole_set_vec(vec, pos, value); \
            crole_reset_vec_err(error); \
        } else \
            error = CROLE_VEC_OUT_OF_BOUNDS; \
    } while (0)

#define crole_destroy_vec(vec) do { \
        free((vec).array); \
    } while (0)

#define crole_pop_unchecked_vec(vec) ((vec).array[--((vec).length)])

#define crole_pop_vec(vec, type, error) \
    ((vec).length != 0 ? \
        crole_pop_unchecked_vec(vec) \
    : \
        (type)crole_set_vec_err(&error, CROLE_VEC_POP_EMPTY))

#define crole_shrink_vec(vec, type, error) do { \
        (vec).allocated = (vec).length; \
        void *new_array = realloc((vec).array, sizeof(type) * (vec).allocated); \
        if (new_array) \
            (vec).array = new_array; \
        else error = CROLE_VEC_MALLOC_FAIL; \
    } while (0)

#endif
