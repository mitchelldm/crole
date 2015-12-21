#include <crole/ref.h>

#include "test_globals.h"
#include "tests.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

bool destructor_triggered = false;

void ref_test_destructor(void *ptr)
{
    destructor_triggered = true;
    free(ptr);
}

void test_ref_actions()
{
    test_section("Reference counting");

    int *number = malloc(sizeof(int));
    *number = 20;

    crole_ref number_ref;
    crole_init_ref(&number_ref, number, &ref_test_destructor);

    int *ptr_to_number = crole_enter_ref(&number_ref);

    test_assert("Reference count is 1 after 1 reference is made", number_ref.ref_count == 1);
    test_assert("Correct value", *ptr_to_number == 20);

    int *second_ptr = crole_enter_ref(&number_ref);

    test_assert("Reference count increases with enter_ref", number_ref.ref_count == 2);
    test_assert("Correct value in additional reference", *second_ptr == 20);
    crole_leave_ref(&number_ref);

    test_assert("Reference count decreases with leave_ref", number_ref.ref_count == 1);

    crole_leave_ref(&number_ref);

    test_assert("Reference count is 0 after all references are gone", number_ref.ref_count == 0);
    test_assert("Destructor triggered after all references are gone", destructor_triggered == true);
}

void test_ref()
{
    printf("\n========== crole_ref ==========\n");

    test tests[] = {
        &test_ref_actions,    
        NULL
    };

    run_tests(tests);
}