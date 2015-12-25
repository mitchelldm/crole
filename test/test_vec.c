/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <crole/vec.h>

#include "tests.h"

void test_vec_actions()
{
    test_section("Vectors - temporary tests");

    crole_vec vec;
    crole_init_vec(&vec, int);

    crole_append_vec(&vec, int, 14);
    crole_append_vec(&vec, int, 20);

    test_assert("Length is correct", vec.length == 2);
    test_assert("Allocated amount is correct", vec.allocated == 3);

    crole_append_vec(&vec, int, 9);
    crole_append_vec(&vec, int, 24);

    test_assert("Allocated amount doubles on full", vec.allocated == 6);
    int out;
    crole_get_vec(&vec, 2, &out);
    test_assert("Correct value at index", out == 9);

    crole_pop_vec(&vec);
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

