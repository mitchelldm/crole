/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef TESTS_H
#define TESTS_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

uint64_t tests_passed;
uint64_t tests_failed;

// __FILE__ gives full path to file, we only want file name
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#   define ANSI_COLOUR_RED   ""
#   define ANSI_COLOUR_GREEN ""
#   define ANSI_COLOUR_RESET ""
#else
#   define ANSI_COLOUR_RED   "\x1b[31m"
#   define ANSI_COLOUR_GREEN "\x1b[32m"
#   define ANSI_COLOUR_RESET "\x1b[0m"
#endif

#define test_section(name) char test_section_name[] = name; printf("\nSection: %s\n", test_section_name)

#define test_assert(test_name, expression) do { \
        if (!(expression)) { \
            printf(ANSI_COLOUR_RED "\tFailed: %s - assertion (%s) in %s (%s:%d)\n" ANSI_COLOUR_RESET, \
-                test_name, #expression, __func__, __FILENAME__, __LINE__); \
            tests_failed++; \
        } else { \
            printf(ANSI_COLOUR_GREEN "\tPassed: %s\n" ANSI_COLOUR_RESET, test_name); \
            tests_passed++; \
        } \
    } while(0)

typedef void(*test)();

void run_tests(test *tests);



void test_dllist();
void test_ref();
void test_vec();

#endif
