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
	mtx_init(&ref->lock, mtx_plain);
}

void *crole_enter_ref(crole_ref *ref)
{
	if (!ref->value) return NULL;

	mtx_lock(&ref->lock);
    ref->ref_count++;
	mtx_unlock(&ref->lock);

    return ref->value;
}

void crole_leave_ref(crole_ref *ref)
{
	mtx_lock(&ref->lock);
    ref->ref_count--;
	mtx_unlock(&ref->lock);

    if (ref->ref_count == 0) {
        ref->destructor(ref->value);
		ref->value = NULL;
		mtx_destroy(&ref->lock);
    }
}
