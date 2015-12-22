/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <crole/ref.h>

#include <stdlib.h>
#include <string.h>

void crole_init_ref(crole_ref *ref, void *ptr, void (*destructor)(void *))
{
    ref->destructor = destructor;
    ref->ref_count = 0;
    ref->value = ptr;
}

void *crole_enter_ref(crole_ref *ref)
{
    ref->ref_count++;

    return ref->value;
}

void crole_leave_ref(crole_ref *ref)
{
    ref->ref_count--;

    if (ref->ref_count == 0) {
        ref->destructor(ref->value);
    }
}

void crole_generic_destructor_ref(void *ptr)
{
    free(ptr);
}
