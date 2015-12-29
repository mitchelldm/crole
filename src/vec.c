/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <crole/vec.h>

const char *crole_translate_vec_err(crole_vec_err error)
{
    static const char no_err[] = "CROLE_VEC_NO_ERR";
    static const char out_of_bounds[] = "CROLE_VEC_OUT_OF_BOUNDS";
    static const char malloc_fail[] = "CROLE_VEC_MALLOC_FAIL";
    static const char pop_empty[] = "CROLE_VEC_POP_EMPTY";
    static const char unknown[] = "CROLE_VEC_UNKNOWN_ERR";

    switch (error) {
        case CROLE_VEC_NO_ERR:
            return no_err;

        case CROLE_VEC_OUT_OF_BOUNDS:
            return out_of_bounds;

        case CROLE_VEC_MALLOC_FAIL:
            return malloc_fail;

        case CROLE_VEC_POP_EMPTY:
            return pop_empty;

        default:
            return unknown;
    }
}

char crole_set_vec_err(crole_vec_err *error, crole_vec_err err_type)
{
    *error = err_type;
    return 0;
}
