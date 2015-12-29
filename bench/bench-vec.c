/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <crole/vec.h>
#include <stdio.h>

crole_declare_vec(int);

int main()
{
    int val;
    for (int m = 0; m < 10000; m++) {
        crole_vec_err error;
        crole_reset_vec_err(error);
        crole_vec_int vec;
        crole_init_vec(vec, int, error);

        for (int i = 0; i < 100000; i++) {
            crole_append_vec(vec, int, i, error);
        }

        for (int i = 0; i < 100000; i++) {
            val = crole_get_vec(vec, int, i, error);
        }

        for (int i = 0; i < 100000; i++) {
            crole_pop_vec(vec, int, error);
        }

        crole_destroy_vec(vec);

        if (m % 1000 == 0) printf("%d\n", m / 100);
    }

    printf("%d\n", val);
}