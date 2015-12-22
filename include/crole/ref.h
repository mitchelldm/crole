/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CROLE_REF_H
#define CROLE_REF_H

#include <stdint.h>

typedef struct crole_ref {
    void (*destructor)(void *);
    void *value;
    _Atomic uint_fast16_t ref_count;
} crole_ref;

void crole_init_ref(crole_ref *ref, void *ptr, void (*destructor)(void *));

void *crole_enter_ref(crole_ref *ref);

void crole_leave_ref(crole_ref *ref);

void crole_generic_destructor_ref(void *ptr);

#endif
