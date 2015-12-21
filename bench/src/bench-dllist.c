#include <crole/dllist.h>
#include <stdio.h>

int main()
{
    int val = 0;
    
    for (int m = 0; m < 10000; m++) {
        crole_dllist_err error;
        crole_dllist list;
        crole_init_dllist(&list, int);
        
        for (int j = 0; j < 1000; j++) {
            crole_push_ptr_back_dllist(&list, &j, &error);
        }
        
        for (int j = 0; j < 1000; j++) {
            crole_push_ptr_front_dllist(&list, &j, &error);
        }
        
        while (list.length > 1000) {
            crole_pop_back_dllist(&list, &error);
        }
        
        while (list.length > 0) {
            crole_pop_front_dllist(&list, &error);
        }

        for (int i = 0; i < 1000; i++) {
            crole_push_ptr_back_dllist(&list, &i, &error);
        }

        for (int i = 0; i < 1000; i++) {
            crole_get_dllist(&list, 20, &val, &error);
        }

        for (int i = 0; i < 1000; i++) {
            crole_insert_ptr_dllist(&list, 20, &i, &error);
        }

        for (int i = 0; i < 1000; i++) {
            crole_remove_dllist(&list, 20, NULL, &error);
        }

       for (int i = 0; i < 1000; i++) {
            crole_foreach_dllist(&list, int, j) {
                val = *j;
            }

            crole_foreach_reverse_dllist(&list, int, k) {
                val = *k;
            }
        }

        crole_destroy_dllist(&list);

        if (m % 1000 == 0) printf("%d\n", m / 100);
    }

    printf("%d\n", val);
}
