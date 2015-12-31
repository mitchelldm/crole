/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CROLE_REF_H
#define CROLE_REF_H

#include <stdint.h>

#if defined _WIN32 || _WIN64
#   define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#   define CROLE_REF_WINDOWS
#endif

// crole_ref is threadsafe and so may be passed to multiple threads freely,
// however it does not automatically make the value it contains threadsafe.
// Will be destroyed when no references remain.
typedef struct crole_ref {
    void (*destructor)(void *);
    void *value;
#ifdef CROLE_REF_WINDOWS
    uint_fast32_t ref_count;
#else
    _Atomic uint_fast32_t ref_count;
#endif
} crole_ref;

// Initialise a reference. Pointer points to memory to manage, destructor
// points to destructor to be called - pass "&free" to simply free the memory
void crole_init_ref(crole_ref *ref, void *ptr, void (*destructor)(void *));

// Increment the reference count and get a pointer to the value in the reference.
// Typically called at the start of a function. Return NULL if reference already destroyed.
void *crole_enter_ref(crole_ref *ref);

// Decrement the reference count, possibly destroying both the value and the reference itself.
void crole_leave_ref(crole_ref *ref);

#endif
