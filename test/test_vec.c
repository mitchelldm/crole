/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <crole/vec.h>

#include "tests.h"

crole_declare_vec(int);

void test_vec_actions()
{
    test_section("Vectors - temporary tests");

    crole_vec_err error;
    crole_reset_vec_err(error);

    crole_vec_int vec;
    crole_init_vec(vec, int, error);

    crole_append_vec(vec, int, 14, error);
    crole_append_vec(vec, int, 20, error);

    test_assert("Length is correct", vec.length == 2);
    test_assert("Allocated amount is correct", vec.allocated == 2);

    crole_append_vec(vec, int, 9, error);
    crole_append_vec(vec, int, 24, error);

    test_assert("Allocated amount doubles on full", vec.allocated == 4);
    int out = crole_get_vec(vec, int, 2, error);
    test_assert("Correct value at index", out == 9);

    crole_pop_vec(vec, int, error);
    test_assert("Popping shortens vec", vec.length == 3);
}

void test_vec()
{
    printf("\n========== crole_vec ==========\n");

    test tests[] = {
        &test_vec_actions,
        NULL
    };

    run_tests(tests);
}
