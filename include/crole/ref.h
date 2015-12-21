#ifndef CROLE_REF_H
#define CROLE_REF_H

#include <stdint.h>

typedef struct crole_ref {
    void (*destructor)(void *);
    _Atomic uint_fast16_t ref_count;
    void *value;
} crole_ref;

crole_ref *crole_init_ref(crole_ref *ref, void *ptr, void (*destructor)(void *));

void *crole_enter_ref(crole_ref *ref);

void crole_leave_ref(crole_ref *ref);

void crole_generic_destructor_ref(void *ptr);

#endif