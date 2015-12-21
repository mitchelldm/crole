#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct int_list_node {
    int value;
    struct int_list_node *next;
    struct int_list_node *prev;
} int_list_node;

typedef struct {
    uint64_t length;
    int_list_node *start;
    int_list_node *end;
} int_list;

int main()
{
    int val = 0;

    for (int m = 0; m < 100000; m++) {
        int_list *list = malloc(sizeof(list));

        for (int j = 0; j < 1000; j++) {
            int_list_node *new_node = malloc(sizeof(int_list_node));
            new_node->value = j;
            new_node->prev = list->end;
            new_node->next = NULL;
            if (list->end)
                list->end->next = new_node;
            list->end = new_node;
            
            list->length++;
        }

        for (int j = 0; j < 1000; j++) {
            int_list_node *new_node = malloc(sizeof(int_list_node));
            new_node->value = j;
            new_node->prev = NULL;
            new_node->next = list->start;
            if (list->start)
                list->start->prev = new_node;
            list->start = new_node;
            
            list->length++;
        }

        while (list->length > 1000) {
            int_list_node *node = list->end;
            if (node) {
                list->end = node->prev;
                
                if (list->end)
                    list->end->next = NULL;
                else
                    list->start = NULL;
    
                free(node);
                list->length--;
            }
        }
        puts("hello\n");
        while (list->length > 1) {
            int_list_node *node = list->start;
            printf("%p\n", node);
            if (node) {
                puts("hello\n");
                list->start = node->next;

                if (list->start)
                    list->start->prev = NULL;
                else
                    list->end = NULL;

                free(node);
                list->length--;
            }
        }

        for (int j = 0; j < 1000; j++) {
            int_list_node *new_node = malloc(sizeof(int_list_node));
            new_node->value = j;
            new_node->prev = NULL;
            new_node->next = list->start;
            list->start->prev = new_node;
            list->start = new_node;

            list->length++;
        }

        for (int i = 0; i < 1000; i++) {
            int_list_node *node = list->start;
            for (uint64_t pos = 0; pos < 20; pos++) {
                node = node->next;
            }
            val = node->value;
        }

        for (int i = 0; i < 1000; i++) {
            int_list_node *new_node = malloc(sizeof(int_list_node));
            new_node->value = i;

            int_list_node *node = list->start;
            for (uint64_t pos = 0; pos < 20; pos++) {
                node = node->next;
            }

            new_node->prev = node->prev;
            new_node->next = node;

            node->prev = new_node;
            new_node->prev->next = new_node;

            list->length++;
        }

        for (int i = 0; i < 1000; i++) {
            int_list_node *node = list->start;
            for (uint64_t pos = 0; pos < 20; pos++) {
                node = node->next;
            }

            node->next->prev = node->prev;
            node->prev->next = node->next;

            free(node);
            list->length--;
        }

        for (int i = 0; i < 1000; i++) {
            for (int_list_node *node = list->start; node; node = node->next) {
                val = node->value;
            }

            for (int_list_node *node = list->end; node; node = node->prev) {
                val = node->value;
            }
        }
        if (m % 1000 == 0) printf("%d\n", m / 100);
    }
    printf("%d\n", val);
}