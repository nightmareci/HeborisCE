#ifndef APP_hashtable_h_
#define APP_hashtable_h_

#include <SDL3/SDL.h>

typedef struct APP_HashTable APP_HashTable;

typedef void (*APP_DestroyHashTableValueCallback)(uint64_t key, void* value, void* userdata);

typedef SDL_EnumerationResult (*APP_EnumerateHashTableCallback)(uint64_t key, void* value, void* userdata);

typedef enum APP_SetHashTableStatus
{
	APP_SET_HASH_TABLE_STATUS_FAILURE = 0,
	APP_SET_HASH_TABLE_STATUS_INSERTED = 1,
	APP_SET_HASH_TABLE_STATUS_UPDATED = 2
} APP_SetHashTableStatus;

APP_HashTable* APP_CreateHashTable(size_t initialCapacity);

bool APP_SetHashTableUserdata(APP_HashTable* table, void* userdata);

APP_SetHashTableStatus APP_SetHashTableValue(APP_HashTable* table, uint64_t key, void* value, APP_DestroyHashTableValueCallback destroy, void** oldValue);

bool APP_GetHashTableValue(APP_HashTable* table, uint64_t key, void** value);

bool APP_RemoveHashTableValue(APP_HashTable* table, uint64_t key, bool* removed);

bool APP_HashTableHasValue(APP_HashTable* table, uint64_t key);

size_t APP_GetHashTableSize(APP_HashTable* table);

bool APP_EnumerateHashTable(APP_HashTable* table, APP_EnumerateHashTableCallback callback);

void APP_DestroyHashTable(APP_HashTable* table);

#endif
