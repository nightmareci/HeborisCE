/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2024 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/
#ifndef HT_hashtable_h_
#define HT_hashtable_h_

#include "SDL_stdinc.h"

/**
 * Taken from the SDL3 source code, ported to work in SDL2. Just drop
 * HT_hashtable.h and HT_hashtable.c into your project to use these hash table
 * APIs. The namespace-prefix has been changed from SDL_ to HT_, so as to not
 * clash with SDL.
 *
 * -nightmareci
 */

struct HT_HashTable;
typedef struct HT_HashTable HT_HashTable;
typedef Uint32 (*HT_HashTable_HashFn)(const void *key, void *data);
typedef SDL_bool (*HT_HashTable_KeyMatchFn)(const void *a, const void *b, void *data);
typedef void (*HT_HashTable_NukeFn)(const void *key, const void *value, void *data);

HT_HashTable *HT_CreateHashTable(void *data,
                                   const Uint32 num_buckets,
                                   const HT_HashTable_HashFn hashfn,
                                   const HT_HashTable_KeyMatchFn keymatchfn,
                                   const HT_HashTable_NukeFn nukefn,
                                   const SDL_bool stackable);

void HT_EmptyHashTable(HT_HashTable *table);
void HT_DestroyHashTable(HT_HashTable *table);
SDL_bool HT_InsertIntoHashTable(HT_HashTable *table, const void *key, const void *value);
SDL_bool HT_RemoveFromHashTable(HT_HashTable *table, const void *key);
SDL_bool HT_FindInHashTable(const HT_HashTable *table, const void *key, const void **_value);
SDL_bool HT_HashTableEmpty(HT_HashTable *table);

/* iterate all values for a specific key. This only makes sense if the hash is stackable. If not-stackable, just use HT_FindInHashTable(). */
SDL_bool HT_IterateHashTableKey(const HT_HashTable *table, const void *key, const void **_value, void **iter);

/* iterate all key/value pairs in a hash (stackable hashes can have duplicate keys with multiple values). */
SDL_bool HT_IterateHashTable(const HT_HashTable *table, const void **_key, const void **_value, void **iter);

Uint32 HT_HashString(const void *key, void *unused);
SDL_bool HT_KeyMatchString(const void *a, const void *b, void *unused);

Uint32 HT_HashID(const void *key, void *unused);
SDL_bool HT_KeyMatchID(const void *a, const void *b, void *unused);

#endif /* HT_hashtable_h_ */
