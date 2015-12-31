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
    if (!ref->value) return NULL;

#ifdef CROLE_REF_WINDOWS
    IterlockedIncrement(&ref->ref_count);
#else
    ref->ref_count++;
#endif

    return ref->value;
}

void crole_leave_ref(crole_ref *ref)
{
#ifdef CROLE_REF_WINDOWS
    IterlockedDecrement(&ref->ref_count);
#else
    ref->ref_count--;
#endif
    if (ref->ref_count == 0) {
        ref->destructor(ref->value);
        ref->value = NULL;
    }
}
