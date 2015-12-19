#include "test_globals.h"

#include "dllist.h"

void test_init_dllist_size()
{
    test_section("Init dllist with element size");
    
    dllist list;
    init_dllist_size(&list, 4);
    
    test_assert("Correct element size", list.size == 4);
}

void test_init_dllist()
{
    test_section("Init dllist with type");

    dllist list;
    init_dllist(&list, int);
    
    test_assert("Correct element size", list.size == sizeof(int));
}

void test_push_ptr_back_dllist()
{
    test_section("Push value to back of dllist from pointer");

    dllist_err error;
    dllist list;
    init_dllist(&list, int);

    int my_value = 4;
    push_ptr_back_dllist(&list, &my_value, &error);
    
    test_assert("Correct list length", list.length == 1);
    
    int read_value;
    get_dllist(&list, 0, &read_value, &error);
    
    test_assert("Correct value at position", read_value == my_value);
    
    int another_value = 20;
    push_ptr_back_dllist(&list, &another_value, &error);
    
    get_dllist(&list, 1, &read_value, &error);
    
    test_assert("Pushes to end of list", read_value == another_value);
    test_assert("Grows list length", list.length == 2);
    
    destroy_dllist(&list);
}

void test_push_back_dllist()
{
    test_section("Push value to back of dllist");

    dllist_err error;
    dllist list;
    init_dllist(&list, int);

    push_back_dllist(&list, int, 7, &error);
    
    test_assert("Correct list length", list.length == 1);
    
    int read_value;
    get_dllist(&list, 0, &read_value, &error);
    
    test_assert("Correct value at position", read_value == 7);
    
    push_back_dllist(&list, int, 19, &error);
    get_dllist(&list, 1, &read_value, &error);
    
    test_assert("Pushes to end of list", read_value == 19);
    test_assert("Grows list length", list.length == 2);
    
    destroy_dllist(&list);
}

void test_dllist()
{
    test tests[] = {
        &test_init_dllist_size,
        &test_push_ptr_back_dllist,
        &test_push_back_dllist,
        NULL
    };
    
    run_tests(tests);
}
