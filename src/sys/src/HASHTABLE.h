#ifndef HASHTABLE_h_
#define HASHTABLE_h_

#include <SDL3/SDL.h>

typedef struct HASHTABLE_Object HASHTABLE_Object;

typedef void (*HASHTABLE_DestroyValueCallback)(uint64_t key, void* value, void* userdata);

typedef SDL_EnumerationResult (*HASHTABLE_EnumerateCallback)(uint64_t key, void* value, void* userdata);

typedef enum HASHTABLE_SetStatus
{
	HASHTABLE_SET_STATUS_FAILURE = 0,
	HASHTABLE_SET_STATUS_INSERTED = 1,
	HASHTABLE_SET_STATUS_UPDATED = 2
} HASHTABLE_SetStatus;

HASHTABLE_Object* HASHTABLE_Create(size_t initialCapacity);

bool HASHTABLE_SetUserdata(HASHTABLE_Object* table, void* userdata);

HASHTABLE_SetStatus HASHTABLE_SetValue(HASHTABLE_Object* table, uint64_t key, void* value, HASHTABLE_DestroyValueCallback destroy, void** oldValue);

bool HASHTABLE_GetValue(HASHTABLE_Object* table, uint64_t key, void** value);

bool HASHTABLE_RemoveValue(HASHTABLE_Object* table, uint64_t key, bool* removed);

bool HASHTABLE_HasValue(HASHTABLE_Object* table, uint64_t key);

size_t HASHTABLE_GetSize(HASHTABLE_Object* table);

bool HASHTABLE_Enumerate(HASHTABLE_Object* table, HASHTABLE_EnumerateCallback callback);

void HASHTABLE_Empty(HASHTABLE_Object* table);

void HASHTABLE_Destroy(HASHTABLE_Object* table);

#endif
