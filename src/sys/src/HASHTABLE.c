#include "HASHTABLE.h"
#include <MAIN.h>
#include <ERROR.h>
#if defined(_M_X64) || defined(_M_ARM64)
#include <intrin.h>
#endif

typedef uint8_t HASHTABLE_SlotState;

typedef struct HASHTABLE_Slot
{
	uint64_t key;
	void* value;
	HASHTABLE_DestroyValueCallback destroy;
	bool used;
} HASHTABLE_Slot;

struct HASHTABLE_Object
{
	HASHTABLE_Slot* slots;
	void* userdata;
	size_t capacity;
	size_t size;
	size_t resizeThreshold;
};

#define HASH_TABLE_MAX_CAPACITY (SIZE_MAX / sizeof(HASHTABLE_Slot))

#define HASH_TABLE_MAX_LOAD_NUMERATOR 19
#define HASH_TABLE_MAX_LOAD_RIGHT_SHIFT 5

SDL_FORCE_INLINE size_t HASHTABLE_GetResizeThreshold(size_t capacity)
{
	if (capacity >= HASH_TABLE_MAX_CAPACITY) {
		ERROR_Set("Hash table capacity is too large to be resized");
		MAIN_Exit(SDL_APP_FAILURE);
	}

#if SIZE_MAX > UINT64_MAX
	#error Current SIZE_MAX is unsupported, 64-bit is the maximum supported!

#elif SIZE_MAX > UINT32_MAX
#ifdef __SIZEOF_INT128__
	// GNU C
	return (size_t)(((__int128)capacity * HASH_TABLE_MAX_LOAD_NUMERATOR) >> HASH_TABLE_MAX_LOAD_RIGHT_SHIFT);

#elif defined(_M_X64) || defined(_M_ARM64)
	// MSVC
	const unsigned __int64 low = (unsigned __int64)capacity * HASH_TABLE_MAX_LOAD_NUMERATOR;
	const unsigned __int64 high = __umulh(capacity, HASH_TABLE_MAX_LOAD_NUMERATOR);
	return (size_t)((high << (64 - HASH_TABLE_MAX_LOAD_RIGHT_SHIFT)) | (low >> HASH_TABLE_MAX_LOAD_RIGHT_SHIFT));

#else
	// Portable
	uint64_t op1 = HASH_TABLE_MAX_LOAD_NUMERATOR;
	uint64_t op2 = capacity;
	uint64_t u1 = op1 & 0xFFFFFFFFu;
	uint64_t v1 = op2 & 0xFFFFFFFFu;
	uint64_t t = u1 * v1;
	uint64_t w3 = t & 0xFFFFFFFFu;
	uint64_t k = t >> 32;
	op1 >>= 32;
	t = (op1 * v1) + k;
	k = t & 0xFFFFFFFFu;
	uint64_t w1 = t >> 32;
	op2 >>= 32;
	t = (u1 * op2) + k;
	k = t >> 32;
	const uint64_t low = (t << 32) + w3;
	const uint64_t high = (op1 * op2) + w1 + k;
	return (size_t)((high << (64 - HASH_TABLE_MAX_LOAD_RIGHT_SHIFT)) | (low >> HASH_TABLE_MAX_LOAD_RIGHT_SHIFT));
#endif
#else
	return (size_t)(((uint64_t)capacity * HASH_TABLE_MAX_LOAD_NUMERATOR) >> HASH_TABLE_MAX_LOAD_RIGHT_SHIFT);

#endif
}

#define HASHTABLE_MIXUINT64_C1 UINT64_C(0x9E3779B97F4A7C15)
#define HASHTABLE_MIXUINT64_C2 UINT64_C(0xBF58476D1CE4E5B9)
#define HASHTABLE_MIXUINT64_C3 UINT64_C(0x94D049BB133111EB)

SDL_FORCE_INLINE uint64_t HASHTABLE_GetKeyHash(uint64_t key)
{
	key += HASHTABLE_MIXUINT64_C1;
	key = (key ^ (key >> 30)) * HASHTABLE_MIXUINT64_C2;
	key = (key ^ (key >> 27)) * HASHTABLE_MIXUINT64_C3;
	key = key ^ (key >> 31);
	return key;
}

static size_t HASHTABLE_GetPowerOfTwoCapacity(size_t capacity)
{
	if (capacity == 0) {
		return 1;
	}
	--capacity;
	capacity |= capacity >> 1;
	capacity |= capacity >> 2;
	capacity |= capacity >> 4;
	capacity |= capacity >> 8;
	capacity |= capacity >> 16;
#if SIZE_MAX > UINT32_MAX
	capacity |= capacity >> 32;
#elif SIZE_MAX > UINT64_MAX
	#error Maximum supported SIZE_MAX is UINT64_MAX!
#endif
	return ++capacity;
}

static HASHTABLE_Slot* HASHTABLE_AllocSlots(size_t capacity)
{
	if (capacity > SIZE_MAX / sizeof(HASHTABLE_Slot)) {
		return NULL;
	}
	return (HASHTABLE_Slot*)SDL_calloc(capacity, sizeof(HASHTABLE_Slot));
}

HASHTABLE_Object* HASHTABLE_Create(size_t initialCapacity)
{
	if (initialCapacity < 16) {
		initialCapacity = 16;
	}
	initialCapacity = HASHTABLE_GetPowerOfTwoCapacity(initialCapacity);

	HASHTABLE_Slot* const slots = HASHTABLE_AllocSlots(initialCapacity);
	if (!slots) {
		ERROR_Set("Failed allocating slots for creating a hash table");
		return NULL;
	}

	HASHTABLE_Object* const table = (HASHTABLE_Object*)SDL_malloc(sizeof(HASHTABLE_Object));
	if (table == NULL) {
		ERROR_Set("Failed allocating hash table");
		SDL_free(slots);
		return NULL;
	}

	table->slots = slots;
	table->userdata = NULL;
	table->capacity = initialCapacity;
	table->size = 0;
	table->resizeThreshold = HASHTABLE_GetResizeThreshold(initialCapacity);
	return table;
}

bool HASHTABLE_SetUserdata(HASHTABLE_Object* table, void* userdata)
{
	if (!table) {
		return ERROR_Set("Invalid parameter \"table\"");
	}
	table->userdata = userdata;
	return true;
}

static bool HASHTABLE_Rehash(HASHTABLE_Object* table, size_t newCapacity)
{
	HASHTABLE_Slot* const newSlots = HASHTABLE_AllocSlots(newCapacity);
	if (!newSlots) {
		return ERROR_Set("Failed allocating new slots for rehashing a hash table");
	}

	const size_t oldCapacity = table->capacity;
	HASHTABLE_Slot* const oldSlots = table->slots;

#define HASHTABLE_REHASH(firstIndex, nextIndex) \
	for (size_t i = 0; i < oldCapacity; ++i) { \
		if (oldSlots[i].used) { \
			const uint64_t key = oldSlots[i].key; \
			void* const value = oldSlots[i].value; \
			HASHTABLE_DestroyValueCallback const destroy = oldSlots[i].destroy; \
			const uint64_t hash = HASHTABLE_GetKeyHash(key); \
			size_t index = (size_t)(firstIndex); \
			while (true) { \
				if (!newSlots[index].used) { \
					newSlots[index].used = true; \
					newSlots[index].key = key; \
					newSlots[index].value = value; \
					newSlots[index].destroy = destroy; \
					break; \
				} \
				index = (nextIndex); \
			} \
		} \
	}

	if (newCapacity == HASH_TABLE_MAX_CAPACITY) {
		HASHTABLE_REHASH(
			hash % HASH_TABLE_MAX_CAPACITY,
			(index + 1) % HASH_TABLE_MAX_CAPACITY
		);
	}
	else {
		const size_t mask = newCapacity - 1;
		HASHTABLE_REHASH(
			hash & mask,
			(index + 1) & mask
		);
	}

	SDL_free(oldSlots);
	table->slots = newSlots;
	table->capacity = newCapacity;
	table->resizeThreshold = HASHTABLE_GetResizeThreshold(newCapacity);
	return true;
}

static bool HASHTABLE_EnsureCapacityForSet(HASHTABLE_Object* table)
{
	if (table->size >= table->resizeThreshold) {
		size_t newCapacity = table->capacity * 2;
		if (table->capacity >= HASH_TABLE_MAX_CAPACITY) {
			ERROR_Set("Hash table capacity is too large to be resized");
		}
		else if (table->capacity > HASH_TABLE_MAX_CAPACITY / 2) {
			newCapacity = HASH_TABLE_MAX_CAPACITY;
		}
		return HASHTABLE_Rehash(table, newCapacity);
	}
	return true;
}

static size_t HASHTABLE_FindSlotIndex(HASHTABLE_Object* table, uint64_t key, bool* used)
{
	const uint64_t hash = HASHTABLE_GetKeyHash(key);

#define HASHTABLE_FIND_SLOT_INDEX(firstIndex, nextIndex) \
	size_t index = (size_t)(firstIndex); \
	while (true) { \
		if (table->slots[index].used) { \
			if (table->slots[index].key == key) { \
				*used = true; \
				return index; \
			} \
		} \
		else { \
			*used = false; \
			return index; \
		} \
		index = (nextIndex); \
	}

	if (table->capacity == HASH_TABLE_MAX_CAPACITY) {
		HASHTABLE_FIND_SLOT_INDEX(
			hash % HASH_TABLE_MAX_CAPACITY,
			(index + 1) % HASH_TABLE_MAX_CAPACITY
		);
	}
	else {
		const size_t mask = table->capacity - 1;
		HASHTABLE_FIND_SLOT_INDEX(
			hash & mask,
			(index + 1) & mask
		);
	}
}

HASHTABLE_SetStatus HASHTABLE_SetValue(HASHTABLE_Object* table, uint64_t key, void* value, HASHTABLE_DestroyValueCallback destroy, void** oldValue)
{
	if (!table) {
		ERROR_Set("Invalid parameter \"table\"");
		return HASHTABLE_SET_STATUS_FAILURE;
	}
	else if (!HASHTABLE_EnsureCapacityForSet(table)) {
		return HASHTABLE_SET_STATUS_FAILURE;
	}

	bool used;
	const size_t index = HASHTABLE_FindSlotIndex(table, key, &used);
	if (used) {
		if (oldValue) {
			*oldValue = table->slots[index].value;
		}
		table->slots[index].value = value;
		table->slots[index].destroy = destroy;
		return HASHTABLE_SET_STATUS_UPDATED;
	}
	else {
		table->slots[index].used = true;
		table->slots[index].key = key;
		table->slots[index].value = value;
		table->slots[index].destroy = destroy;
		++table->size;
		return HASHTABLE_SET_STATUS_INSERTED;
	}
}

bool HASHTABLE_GetValue(HASHTABLE_Object* table, uint64_t key, void** value)
{
	if (!table) {
		false;
	}
	bool used;
	const size_t index = HASHTABLE_FindSlotIndex(table, key, &used);
	if (used) {
		if (value) {
			*value = table->slots[index].value;
		}
		return true;
	}
	else {
		return false;
	}
}

bool HASHTABLE_RemoveValue(HASHTABLE_Object* table, uint64_t key, bool* removed)
{
	if (removed) {
		*removed = false;
	}
	if (!table) {
		return ERROR_Set("Invalid parameter \"table\"");
	}
	bool used;
	const size_t index = HASHTABLE_FindSlotIndex(table, key, &used);
	if (!used) {
		return true;
	}

	if (table->slots[index].destroy) {
		table->slots[index].destroy(table->slots[index].key, table->slots[index].value, table->userdata);
	}
	table->slots[index].used = false;
	if (removed) {
		*removed = true;
	}
	--table->size;
	return true;
}

bool HASHTABLE_HasValue(HASHTABLE_Object* table, uint64_t key)
{
	if (!table) {
		return false;
	}
	return HASHTABLE_GetValue(table, key, NULL);
}

size_t HASHTABLE_GetSize(HASHTABLE_Object* table)
{
	if (!table) {
		return 0;
	}
	return table->size;
}

bool HASHTABLE_Enumerate(HASHTABLE_Object* table, HASHTABLE_EnumerateCallback callback)
{
	if (!table) {
		return ERROR_Set("Invalid parameter \"table\"");
	}
	else if (!callback) {
		return ERROR_Set("Invalid parameter \"callback\"");
	}
	for (size_t i = 0; i < table->capacity; ++i) {
		if (table->slots[i].used) {
			switch (callback(table->slots[i].key, table->slots[i].value, table->userdata)) {
			case SDL_ENUM_CONTINUE:
				continue;

			case SDL_ENUM_SUCCESS:
				return true;

			default:
				return false;
			}
		}
	}
	return true;
}

void HASHTABLE_Empty(HASHTABLE_Object* table)
{
	if (!table) {
		return;
	}
	for (size_t i = 0; i < table->capacity; i++) {
		if (table->slots[i].used && table->slots[i].destroy) {
			table->slots[i].destroy(table->slots[i].key, table->slots[i].value, table->userdata);
		}
		table->slots[i].used = false;
	}
	table->size = 0;
}

void HASHTABLE_Destroy(HASHTABLE_Object* table)
{
	HASHTABLE_Empty(table);
	if (table) {
		SDL_free(table->slots);
	}
	SDL_free(table);
}
