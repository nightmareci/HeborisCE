#include "APP_hashtable.h"
#include "APP_main.h"
#include "APP_error.h"
#if defined(_M_X64) || defined(_M_ARM64)
#include <intrin.h>
#endif

typedef uint8_t APP_HashTableSlotState;

typedef struct APP_HashTableSlot
{
	uint64_t key;
	void* value;
	APP_DestroyHashTableValueCallback destroy;
	bool used;
} APP_HashTableSlot;

struct APP_HashTable
{
	APP_HashTableSlot* slots;
	void* userdata;
	size_t capacity;
	size_t size;
	size_t resizeThreshold;
};

#define APP_HASH_TABLE_MAX_CAPACITY (SIZE_MAX / sizeof(APP_HashTableSlot))

#define APP_HASH_TABLE_MAX_LOAD_NUMERATOR 19
#define APP_HASH_TABLE_MAX_LOAD_RIGHT_SHIFT 5

SDL_FORCE_INLINE size_t APP_GetHashTableResizeThreshold(size_t capacity)
{
	if (capacity >= APP_HASH_TABLE_MAX_CAPACITY) {
		APP_SetError("Hash table capacity is too large to be resized");
		APP_Exit(SDL_APP_FAILURE);
	}

#if SIZE_MAX > UINT64_MAX
	#error Current SIZE_MAX is unsupported, 64-bit is the maximum supported!

#elif SIZE_MAX > UINT32_MAX
#ifdef __SIZEOF_INT128__
	// GNU C
	return (size_t)(((__int128)capacity * APP_HASH_TABLE_MAX_LOAD_NUMERATOR) >> APP_HASH_TABLE_MAX_LOAD_RIGHT_SHIFT);

#elif defined(_M_X64) || defined(_M_ARM64)
	// MSVC
	const unsigned __int64 low = (unsigned __int64)capacity * APP_HASH_TABLE_MAX_LOAD_NUMERATOR;
	const unsigned __int64 high = __umulh(capacity, APP_HASH_TABLE_MAX_LOAD_NUMERATOR);
	return (size_t)((high << (64 - APP_HASH_TABLE_MAX_LOAD_RIGHT_SHIFT)) | (low >> APP_HASH_TABLE_MAX_LOAD_RIGHT_SHIFT));

#else
	// Portable
	uint64_t op1 = APP_HASH_TABLE_MAX_LOAD_NUMERATOR;
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
	return (size_t)((high << (64 - APP_HASH_TABLE_MAX_LOAD_RIGHT_SHIFT)) | (low >> APP_HASH_TABLE_MAX_LOAD_RIGHT_SHIFT));
#endif
#else
	return (size_t)(((uint64_t)capacity * APP_HASH_TABLE_MAX_LOAD_NUMERATOR) >> APP_HASH_TABLE_MAX_LOAD_RIGHT_SHIFT);

#endif
}

#define APP_MIXUINT64_C1 UINT64_C(0x9E3779B97F4A7C15)
#define APP_MIXUINT64_C2 UINT64_C(0xBF58476D1CE4E5B9)
#define APP_MIXUINT64_C3 UINT64_C(0x94D049BB133111EB)

SDL_FORCE_INLINE uint64_t APP_GetHashTableKeyHash(uint64_t key)
{
	key += APP_MIXUINT64_C1;
	key = (key ^ (key >> 30)) * APP_MIXUINT64_C2;
	key = (key ^ (key >> 27)) * APP_MIXUINT64_C3;
	key = key ^ (key >> 31);
	return key;
}

static size_t APP_GetPowerOfTwoHashTableCapacity(size_t capacity)
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

static APP_HashTableSlot* APP_AllocHashTableSlots(size_t capacity)
{
	return (APP_HashTableSlot*)SDL_calloc(capacity, sizeof(APP_HashTableSlot));
}

APP_HashTable* APP_CreateHashTable(size_t initialCapacity)
{
	if (initialCapacity < 16) {
		initialCapacity = 16;
	}
	initialCapacity = APP_GetPowerOfTwoHashTableCapacity(initialCapacity);

	APP_HashTableSlot* const slots = APP_AllocHashTableSlots(initialCapacity);
	if (!slots) {
		APP_SetError("Failed allocating slots for creating a hash table");
		return NULL;
	}

	APP_HashTable* const table = (APP_HashTable*)SDL_malloc(sizeof(APP_HashTable));
	if (table == NULL) {
		APP_SetError("Failed allocating hash table");
		SDL_free(slots);
		return NULL;
	}

	table->slots = slots;
	table->userdata = NULL;
	table->capacity = initialCapacity;
	table->size = 0;
	table->resizeThreshold = APP_GetHashTableResizeThreshold(initialCapacity);
	return table;
}

bool APP_SetHashTableUserdata(APP_HashTable* table, void* userdata)
{
	if (!table) {
		return APP_SetError("Invalid parameter \"table\"");
	}
	table->userdata = userdata;
	return true;
}

static bool APP_RehashHashTable(APP_HashTable* table, size_t newCapacity)
{
	APP_HashTableSlot* const newSlots = APP_AllocHashTableSlots(newCapacity);
	if (!newSlots) {
		return APP_SetError("Failed allocating new slots for rehashing a hash table");
	}

	const size_t oldCapacity = table->capacity;
	APP_HashTableSlot* const oldSlots = table->slots;

#define APP_REHASH_HASH_TABLE(firstIndex, nextIndex) \
	for (size_t i = 0; i < oldCapacity; ++i) { \
		if (oldSlots[i].used) { \
			const uint64_t key = oldSlots[i].key; \
			void* const value = oldSlots[i].value; \
			APP_DestroyHashTableValueCallback const destroy = oldSlots[i].destroy; \
			const uint64_t hash = APP_GetHashTableKeyHash(key); \
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

	if (newCapacity == APP_HASH_TABLE_MAX_CAPACITY) {
		APP_REHASH_HASH_TABLE(
			hash % APP_HASH_TABLE_MAX_CAPACITY,
			(index + 1) % APP_HASH_TABLE_MAX_CAPACITY
		);
	}
	else {
		const size_t mask = newCapacity - 1;
		APP_REHASH_HASH_TABLE(
			hash & mask,
			(index + 1) & mask
		);
	}

	SDL_free(oldSlots);
	table->slots = newSlots;
	table->capacity = newCapacity;
	table->resizeThreshold = APP_GetHashTableResizeThreshold(newCapacity);
	return true;
}

static bool APP_EnsureHashTableCapacityForSet(APP_HashTable* table)
{
	if (table->size >= table->resizeThreshold) {
		size_t newCapacity = table->capacity * 2;
		if (table->capacity >= APP_HASH_TABLE_MAX_CAPACITY) {
			APP_SetError("Hash table capacity is too large to be resized");
		}
		else if (table->capacity > APP_HASH_TABLE_MAX_CAPACITY / 2) {
			newCapacity = APP_HASH_TABLE_MAX_CAPACITY;
		}
		return APP_RehashHashTable(table, newCapacity);
	}
	return true;
}

static size_t APP_FindHashTableSlotIndex(APP_HashTable* table, uint64_t key, bool* used)
{
	const uint64_t hash = APP_GetHashTableKeyHash(key);

#define APP_FIND_HASH_TABLE_SLOT_INDEX(firstIndex, nextIndex) \
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

	if (table->capacity == APP_HASH_TABLE_MAX_CAPACITY) {
		APP_FIND_HASH_TABLE_SLOT_INDEX(
			hash % APP_HASH_TABLE_MAX_CAPACITY,
			(index + 1) % APP_HASH_TABLE_MAX_CAPACITY
		);
	}
	else {
		const size_t mask = table->capacity - 1;
		APP_FIND_HASH_TABLE_SLOT_INDEX(
			hash & mask,
			(index + 1) & mask
		);
	}
}

APP_SetHashTableStatus APP_SetHashTableValue(APP_HashTable* table, uint64_t key, void* value, APP_DestroyHashTableValueCallback destroy, void** oldValue)
{
	if (!table) {
		APP_SetError("Invalid parameter \"table\"");
		return APP_SET_HASH_TABLE_STATUS_FAILURE;
	}
	else if (!APP_EnsureHashTableCapacityForSet(table)) {
		return APP_SET_HASH_TABLE_STATUS_FAILURE;
	}

	bool used;
	const size_t index = APP_FindHashTableSlotIndex(table, key, &used);
	if (used) {
		if (oldValue) {
			*oldValue = table->slots[index].value;
		}
		table->slots[index].value = value;
		table->slots[index].destroy = destroy;
		return APP_SET_HASH_TABLE_STATUS_UPDATED;
	}
	else {
		table->slots[index].used = true;
		table->slots[index].key = key;
		table->slots[index].value = value;
		table->slots[index].destroy = destroy;
		++table->size;
		return APP_SET_HASH_TABLE_STATUS_INSERTED;
	}
}

bool APP_GetHashTableValue(APP_HashTable* table, uint64_t key, void** value)
{
	if (!table) {
		false;
	}
	bool used;
	const size_t index = APP_FindHashTableSlotIndex(table, key, &used);
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

bool APP_RemoveHashTableValue(APP_HashTable* table, uint64_t key, bool* removed)
{
	if (removed) {
		*removed = false;
	}
	if (!table) {
		return APP_SetError("Invalid parameter \"table\"");
	}
	bool used;
	const size_t index = APP_FindHashTableSlotIndex(table, key, &used);
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

bool APP_HashTableHasValue(APP_HashTable* table, uint64_t key)
{
	if (!table) {
		return false;
	}
	return APP_GetHashTableValue(table, key, NULL);
}

size_t APP_GetHashTableSize(APP_HashTable* table)
{
	if (!table) {
		return 0;
	}
	return table->size;
}

bool APP_EnumerateHashTable(APP_HashTable* table, APP_EnumerateHashTableCallback callback)
{
	if (!table) {
		return APP_SetError("Invalid parameter \"table\"");
	}
	else if (!callback) {
		return APP_SetError("Invalid parameter \"callback\"");
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

void APP_DestroyHashTable(APP_HashTable* table)
{
	if (!table) {
		return;
	}
	for (size_t i = 0; i < table->capacity; i++) {
		if (table->slots[i].used && table->slots[i].destroy) {
			table->slots[i].destroy(table->slots[i].key, table->slots[i].value, table->userdata);
		}
	}
	SDL_free(table->slots);
	SDL_free(table);
}
