/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CROLE_VEC_H
#define CROLE_VEC_H

#include <stdlib.h>
#include <stdint.h>

typedef enum {
    CROLE_VEC_NO_ERR = 0,
    CROLE_VEC_OUT_OF_BOUNDS = -1,
    CROLE_VEC_MALLOC_FAIL = -2,
    CROLE_VEC_POP_EMPTY = -3
} crole_vec_err;

const char *crole_translate_vec_err(crole_vec_err error);

#define crole_is_err_vec(error) ((error) != CROLE_VEC_NO_ERR)

#define crole_reset_vec_err(error) ((error) = CROLE_VEC_NO_ERR)

typedef struct {
    void *array;
    size_t allocated;
    size_t length;
    uint_fast16_t size;
} crole_vec;

#define CROLE_VEC_DEFAULT_START_ALLOC 3

void crole_init_vec_size_length(crole_vec *vec, uint_fast16_t size, size_t length);

#define crole_init_vec_length(vec, type, length) crole_init_vec_size_length(vec, sizeof(type), length)

void crole_init_vec_size(crole_vec *vec, uint_fast16_t size);

#define crole_init_vec(vec, type) crole_init_vec_size(vec, sizeof(type))

void crole_append_ptr_vec(crole_vec *vec, void *val_ptr);

#define crole_append_vec(vec, type, value) crole_append_ptr_vec(vec, &(type){value})

void *crole_get_ptr_vec(crole_vec *vec, size_t position);

void crole_get_vec(crole_vec *vec, size_t position, void *out_ptr);

void crole_set_ptr_vec(crole_vec *vec, size_t position, void *val_ptr);

#define crole_set_vec(vec, type, position, value) crole_set_ptr_vec(vec, position, &(type){value})

void crole_pop_vec(crole_vec *vec);

void crole_shrink_vec(crole_vec *vec);

#endif
