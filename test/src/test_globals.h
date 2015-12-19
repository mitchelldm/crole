#ifndef TEST_GLOBALS_H
#define TEST_GLOBALS_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

uint64_t tests_passed;
uint64_t tests_failed;

// __FILE__ gives full path to file, we only want file name
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define ANSI_COLOUR_RED     "\x1b[31m"
#define ANSI_COLOUR_GREEN   "\x1b[32m"
#define ANSI_COLOUR_RESET   "\x1b[0m"

#define test_section(name) char test_section_name[] = name; printf("\nBeginning section: %s\n", test_section_name);

#define test_assert(test_name, expression) do { \
        if (!(expression)) { \
            printf(ANSI_COLOUR_RED "\tFailed: %s - assertion (%s) in %s (%s:%d)\n" ANSI_COLOUR_RESET, \
                test_name, #expression, __func__, __FILENAME__, __LINE__); \
            tests_failed++; \
        } else { \
            printf(ANSI_COLOUR_GREEN "\tPassed: %s\n" ANSI_COLOUR_RESET, test_name); \
            tests_passed++; \
        } \
    } while(0)

typedef void(*test)();

void run_tests(test *tests);

#endif
