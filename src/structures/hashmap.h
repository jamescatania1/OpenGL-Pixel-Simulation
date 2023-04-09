#ifndef _hashmap_h
#define _hashmap_h

typedef struct HashMap {
	int capacity;
	int count;
	void** keys;
	void** values;
}
*HashMap;

typedef struct IntHashMap {
	int capacity;
	int count;
	int* keys;
	void** values;
}
*IntHashMap;

/*
 * HashMap Contstructor for default initial capacity.
 */
extern HashMap new_HashMap();

extern IntHashMap new_IntHashMap();

/*
 * Frees HashMap. Make sure to free all keys/values before calling this if no longer used.
 */
extern void HashMap_free(HashMap map);

extern void IntHashMap_free(IntHashMap map);

/*
 * Inserts a value for given key. If value for key already exists, replaces value.
 */
extern void HashMap_insert(HashMap map, void* key, void* value);

extern void IntHashMap_insert(IntHashMap map, int key, void* value);

/*
 * Returns value for given key.
 */
extern void* HashMap_get(HashMap map, void* key);

extern void* IntHashMap_get(IntHashMap map, int key);

/*
 * Removes key and value at given key. Make sure to free it first if no longer used.
 */
extern void HashMap_remove(HashMap map, void* key);

extern void IntHashMap_remove(IntHashMap map, int key);

#endif