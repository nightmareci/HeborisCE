#include "APP_hashtable.h"
#include "APP_error.h"

typedef enum APP_HashSlotStateEnum
{
	APP_HASH_SLOT_STATE_EMPTY,
	APP_HASH_SLOT_STATE_USED,
	APP_HASH_SLOT_STATE_TOMBSTONE
} APP_HashStateEnum;

typedef uint8_t APP_HashSlotState;

typedef struct APP_HashSlot
{
	uint64_t key;
	void* value;
	APP_DestroyHashTableValueCallback destroy;
	APP_HashSlotState state;
} APP_HashSlot;

struct APP_HashTable
{
	APP_HashSlot* slots;
	void* userdata;
	size_t capacity;
	size_t size;
	size_t tombstones;
	size_t resizeThreshold;
};

#define APP_MAX_LOAD_NUMERATOR ((size_t)6)
#define APP_MAX_LOAD_DENOMINATOR ((size_t)10)

SDL_FORCE_INLINE uint64_t APP_GetHashTableKeyHash(uint64_t key);

static uint64_t APP_GetPowerOfTwoHashTableCapacity(uint64_t x);

static APP_HashSlot* APP_AllocHashSlots(size_t capacity);

static bool APP_RehashHashTable(APP_HashTable* table, size_t newCapacity);

static bool APP_EnsureHashTableCapacityForInsert(APP_HashTable* table);

static size_t APP_FindHashTableSlotIndex(APP_HashTable* table, uint64_t key, bool* used);

#define APP_MIXUINT64_C1 UINT64_C(0x9E3779B97F4A7C15)
#define APP_MIXUINT64_C2 UINT64_C(0xBF58476D1CE4E5B9)
#define APP_MIXUINT64_C3 UINT64_C(0x94D049BB133111EB)

static inline uint64_t APP_GetHashTableKeyHash(uint64_t key)
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

static APP_HashSlot* APP_AllocHashSlots(size_t capacity)
{
	return (APP_HashSlot*)SDL_calloc(capacity, sizeof(APP_HashSlot));
}

APP_HashTable* APP_CreateHashTable(size_t initialCapacity)
{
	size_t capacity = initialCapacity;
	if (capacity == 0) {
		capacity = 16;
	}
	capacity = APP_GetPowerOfTwoHashTableCapacity(capacity);

	APP_HashSlot* const slots = APP_AllocHashSlots(capacity);
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
	table->capacity = capacity;
	table->size = 0;
	table->tombstones = 0;
	table->resizeThreshold = (capacity * APP_MAX_LOAD_NUMERATOR) / APP_MAX_LOAD_DENOMINATOR;
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
	APP_HashSlot* const newSlots = APP_AllocHashSlots(newCapacity);
	if (!newSlots) {
		return APP_SetError("Failed allocating new slots for rehashing a hash table");
	}

	const size_t oldCapacity = table->capacity;
	APP_HashSlot* const oldSlots = table->slots;

	for (size_t i = 0; i < oldCapacity; ++i) {
		if (oldSlots[i].state == APP_HASH_SLOT_STATE_USED) {
			const uint64_t key = oldSlots[i].key;
			void* const value = oldSlots[i].value;
			APP_DestroyHashTableValueCallback const destroy = oldSlots[i].destroy;

			const uint64_t hash = APP_GetHashTableKeyHash(key);
			size_t index = (hash & (newCapacity - 1));
			while (true) {
				if (newSlots[index].state != APP_HASH_SLOT_STATE_USED) {
					newSlots[index].state = APP_HASH_SLOT_STATE_USED;
					newSlots[index].key = key;
					newSlots[index].value = value;
					newSlots[index].destroy = destroy;
					break;
				}
				index = (index + 1) & (newCapacity - 1);
			}
		}
	}

	SDL_free(oldSlots);
	table->slots = newSlots;
	table->capacity = newCapacity;
	table->tombstones = 0;
	table->resizeThreshold = (newCapacity * APP_MAX_LOAD_NUMERATOR) / APP_MAX_LOAD_DENOMINATOR;
	return true;
}

static bool APP_EnsureHashTableCapacityForInsert(APP_HashTable* table)
{
	const size_t combined = table->size + table->tombstones;
	if (combined == SIZE_MAX) {
		return APP_SetError("Out of memory to expand hash table");
	}
	if (combined >= table->resizeThreshold) {
		size_t newCapacity = table->capacity * 2;
		if (newCapacity == 0) {
			newCapacity = 8;
		}
		if (newCapacity < table->capacity) {
			newCapacity = SIZE_MAX;
		}
		return APP_RehashHashTable(table, newCapacity);
	}
	return true;
}

static size_t APP_FindHashTableSlotIndex(APP_HashTable* table, uint64_t key, bool* used)
{
	const uint64_t hash = APP_GetHashTableKeyHash(key);
	const size_t mask = table->capacity - 1;
	size_t index = (size_t)(hash & mask);

	while (true) {
		switch (table->slots[index].state) {
		default:
			*used = false;
			return index;

		case APP_HASH_SLOT_STATE_USED:
			if (table->slots[index].key == key) {
				*used = true;
				return index;
			}
			break;
		}
		index = (index + 1) & mask;
	}
}

APP_SetHashTableStatus APP_SetHashTableValue(APP_HashTable* table, uint64_t key, void* value, APP_DestroyHashTableValueCallback destroy, void** oldValue)
{
	if (!table) {
		APP_SetError("Invalid parameter \"table\"");
		return APP_SET_HASH_TABLE_STATUS_FAILURE;
	}
	else if (!APP_EnsureHashTableCapacityForInsert(table)) {
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
		if (table->slots[index].state == APP_HASH_SLOT_STATE_TOMBSTONE) {
			if (table->tombstones > 0) {
				--table->tombstones;
			}
		}
		table->slots[index].state = APP_HASH_SLOT_STATE_USED;
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
	table->slots[index].state = APP_HASH_SLOT_STATE_TOMBSTONE;
	if (removed) {
		*removed = true;
	}
	--table->size;
	++table->tombstones;
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
		if (table->slots[i].state == APP_HASH_SLOT_STATE_USED) {
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
		if (table->slots[i].state == APP_HASH_SLOT_STATE_USED && table->slots[i].destroy) {
			table->slots[i].destroy(table->slots[i].key, table->slots[i].value, table->userdata);
		}
	}
	SDL_free(table->slots);
	SDL_free(table);
}
