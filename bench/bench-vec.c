#include <crole/vec.h>
#include <stdio.h>

int main()
{
    int val;
    for (int m = 0; m < 10000; m++) {
        crole_vec vec;
        crole_init_vec(&vec, int);

        for (int i = 0; i < 100000; i++) {
            crole_append_ptr_vec(&vec, &i);
        }

        for (int i = 0; i < 100000; i++) {
            crole_get_vec(&vec, i, &val);
        }

        for (int i = 0; i < 100000; i++) {
            crole_pop_vec(&vec);
        }

        crole_destroy_vec(&vec);

        if (m % 1000 == 0) printf("%d\n", m / 100);
    }

    printf("%d\n", val);
}