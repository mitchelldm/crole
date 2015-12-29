/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "tests.h"

#include <crole/dllist.h>

void test_init_dllist_size()
{
    test_section("Init crole_dllist with element size");
    
    crole_dllist list;
    crole_init_dllist_size(&list, 4);
    test_assert("Correct element size", list.size == 4);

    crole_destroy_dllist(&list);
}

void test_init_dllist()
{
    test_section("Init crole_dllist with type");

    crole_dllist list;
    crole_init_dllist(&list, int);
    test_assert("Correct element size", list.size == sizeof(int));

    crole_destroy_dllist(&list);
}

void test_push_ptr_back_dllist()
{
    test_section("Push value to back of crole_dllist from pointer");

    crole_dllist_err error;
    crole_dllist list;
    crole_init_dllist(&list, int);

    int my_value = 4;
    crole_push_ptr_back_dllist(&list, &my_value, &error);
    test_assert("Correct list length", list.length == 1);
    
    int read_value;
    crole_get_dllist(&list, 0, &read_value, &error);
    test_assert("Correct value at position", read_value == my_value);
    
    int another_value = 20;
    crole_push_ptr_back_dllist(&list, &another_value, &error);
    crole_get_dllist(&list, 1, &read_value, &error);
    test_assert("Pushes to end of list", read_value == another_value);
    test_assert("Grows list length", list.length == 2);
    
    crole_destroy_dllist(&list);
}

void test_push_back_dllist()
{
    test_section("Push value to back of crole_dllist");

    crole_dllist_err error;
    crole_dllist list;
    crole_init_dllist(&list, int);

    crole_push_back_dllist(&list, int, 7, &error);
    test_assert("Correct list length", list.length == 1);
    
    int read_value;
    crole_get_dllist(&list, 0, &read_value, &error);
    test_assert("Correct value at position", read_value == 7);
    
    crole_push_back_dllist(&list, int, 19, &error);
    crole_get_dllist(&list, 1, &read_value, &error);
    test_assert("Pushes to end of list", read_value == 19);
    test_assert("Grows list length", list.length == 2);
    
    crole_destroy_dllist(&list);
}

void test_push_ptr_front_dllist()
{
    test_section("Push value to front of crole_dllist from pointer");

    crole_dllist_err error;
    crole_dllist list;
    crole_init_dllist(&list, int);

    int to_push1 = 1;
    crole_push_ptr_front_dllist(&list, &to_push1, &error);
    test_assert("Correct list length", list.length == 1);

    int read_value;
    crole_get_dllist(&list, 0, &read_value, &error);
    test_assert("Correct value at position", read_value == 1);

    int to_push2 = 2;
    crole_push_ptr_front_dllist(&list, &to_push2, &error);
    crole_get_dllist(&list, 0, &read_value, &error);
    test_assert("Pushes to start of list", read_value == 2);
    test_assert("Grows list length", list.length == 2);

    crole_destroy_dllist(&list);
}

void test_push_front_dllist()
{
    test_section("Push value to front of crole_dllist");

    crole_dllist_err error;
    crole_reset_dllist_err(error);
    crole_dllist list;
    crole_init_dllist(&list, int);

    crole_push_front_dllist(&list, int, 1, &error);
    test_assert("Correct list length", list.length == 1);

    int read_value;
    crole_get_dllist(&list, 0, &read_value, &error);
    test_assert("Correct value at position", read_value == 1);

    crole_push_front_dllist(&list, int, 2, &error);
    crole_get_dllist(&list, 0, &read_value, &error);
    test_assert("Pushes to start of list", read_value == 2);
    test_assert("Grows list length", list.length == 2);

    crole_destroy_dllist(&list);
}

void test_init_dllist_from_array_size()
{
    test_section("Init crole_dllist from array");

    crole_dllist_err error;
    crole_reset_dllist_err(error);
    crole_dllist list;

    int numbers[7] = { 0, 1, 2, 3, 4, 5, 6 };
    crole_init_dllist_from_array_size(&list, sizeof(int), numbers, 7, &error);
    test_assert("Correct list length", list.length == 7);
    test_assert("Correct element size", list.size == sizeof(int));

    int read_value;
    crole_get_dllist(&list, 2, &read_value, &error);
    test_assert("Correct value at position", read_value == 2);

    crole_destroy_dllist(&list);
}

void test_dllist()
{
    printf("\n========== crole_dllist ==========\n");

    test tests[] = {
        &test_init_dllist_size,
        &test_push_ptr_back_dllist,
        &test_push_back_dllist,
        &test_push_ptr_front_dllist,
        &test_push_front_dllist,
        &test_init_dllist_from_array_size,
        NULL
    };
    
    run_tests(tests);
}
