#include <crole/vec.h>

#include <string.h>

const char *crole_translate_vec_err(crole_vec_err error)
{
    static const char no_err[] = "CROLE_VEC_NO_ERR";
    static const char out_of_bounds[] = "CROLE_VEC_OUT_OF_BOUNDS";
    static const char malloc_fail[] = "CROLE_VEC_MALLOC_FAIL";
    static const char unknown[] = "CROLE_VEC_UNKNOWN_ERR";

    switch (error) {
	    case CROLE_VEC_NO_ERR:
		    return no_err;

	    case CROLE_VEC_OUT_OF_BOUNDS:
		    return out_of_bounds;

	    case CROLE_VEC_MALLOC_FAIL:
		    return malloc_fail;

	    default:
		    return unknown;
    }
}

void crole_init_vec_size_length(crole_vec *vec, uint_fast16_t size, size_t length)
{
    vec->allocated = length;
    vec->length = 0;
    vec->size = size;
    vec->array = malloc(size * length);
}

void crole_init_vec_size(crole_vec *vec, uint_fast16_t size)
{
    crole_init_vec_size_length(vec, size, CROLE_VEC_DEFAULT_START_ALLOC);
}

void crole_append_ptr_vec(crole_vec *vec, void *val_ptr)
{
    if (vec->length >= vec->allocated) {
        vec->allocated *= 2;
        vec->array = realloc(vec->array, vec->size * vec->allocated);
    }
    void *entry = crole_get_ptr_vec(vec, vec->length++);
    memcpy(entry, val_ptr, vec->size);
}

void *crole_get_ptr_vec(crole_vec *vec, size_t position)
{
    if (position < vec->length) {
        return ((uint8_t *)vec->array) + (position * vec->size);
    } else return NULL; // error
}

void crole_get_vec(crole_vec *vec, size_t position, void *out_ptr)
{
    void *entry = crole_get_ptr_vec(vec, position);
    if (entry)
        memcpy(out_ptr, entry, vec->size);
    else { /* error */ }
}

void crole_set_ptr_vec(crole_vec *vec, size_t position, void *val_ptr)
{
    void *entry = crole_get_ptr_vec(vec, position);
    if (entry)
        memcpy(entry, val_ptr, vec->size);
    else { /* error */}
}

void crole_pop_vec(crole_vec *vec)
{
    if (vec->length > 0) {
        vec->length--;
    } else {
        // error
	}
}

void crole_shrink_vec(crole_vec *vec)
{
    vec->allocated = vec->length;
    vec->array = realloc(vec->array, vec->size * vec->allocated);
}
