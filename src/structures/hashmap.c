#pragma warning( disable: 6385 )
#pragma warning( disable: 6386 )

#include <stdlib.h>
#include <stdio.h>
#include "hashmap.h"

#define HASHMAP_INIT_CAPACITY 32

int hash(void* key);
void resize(HashMap map, int newCapacity);
void resizeInt(IntHashMap map, int newCapacity);

HashMap new_HashMap() {
	HashMap this = (HashMap)malloc(sizeof(struct HashMap));
	if (this == NULL) return NULL;

	this->count = 0;
	this->capacity = HASHMAP_INIT_CAPACITY;
	this->keys = (void**)malloc(this->capacity * sizeof(void*));
	this->values = (void**)malloc(this->capacity * sizeof(void*));
	if (this->keys == NULL || this->values == NULL) return NULL;
	for (int i = 0; i < this->capacity; i++) {
		this->keys[i] = NULL;
		this->values[i] = NULL;
	}

	return this;
}

void HashMap_free(HashMap map) {
	free(map->keys);
	free(map->values);
	free(map);
}

void HashMap_insert(HashMap map, void* key, void* value) {
	if (key == NULL || value == NULL) {
		printf("HashMap error: null key or value.\n");
		exit(EXIT_FAILURE);
	}
	if (map->count >= map->capacity / 2) resize(map, map->capacity * 2);

	int hashVal = hash(key) % map->capacity;
	if (hashVal < 0) hashVal += map->capacity;
	while (map->keys[hashVal] != NULL) {
		hashVal = (hashVal + 1) % map->capacity;
	}
	map->keys[hashVal] = key;
	map->values[hashVal] = value;
	map->count++;
}

void* HashMap_get(HashMap map, void* key) {
	int hashVal = hash(key) % map->capacity;
	if (hashVal < 0) hashVal += map->capacity;
	int start = hashVal;
	while (map->keys[hashVal] != key) {
		hashVal = (hashVal + 1) % map->capacity;
		if (hashVal == start) {
			printf("HashMap error: key does not exist in table.\n");
			exit(EXIT_FAILURE);
		}
	}
	return map->values[hashVal];
}

void HashMap_remove(HashMap map, void* key) {
	int hashVal = hash(key) % map->capacity;
	if (hashVal < 0) hashVal += map->capacity;
	int start = hashVal;
	while (map->keys[hashVal] != key) {
		hashVal = (hashVal + 1) % map->capacity;
		if (hashVal == start) return;
	}
	map->keys[hashVal] = NULL;
	map->values[hashVal] = NULL;
	map->count--;
}

int hash(void* key) {
	int val = (int)key * 2654435761;
	if (val < 0) val *= -1;
	return val;
}

void resize(HashMap map, int newCapacity) {
	void** newKeys = (void**)malloc(newCapacity * sizeof(void*));
	void** newValues = (void**)malloc(newCapacity * sizeof(void*));
	if (newKeys == NULL || newValues == NULL) {
		printf("List error: could not resize HashMap to capacity %d\n", newCapacity);
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < newCapacity; i++) {
		newKeys[i] = NULL;
		newValues[i] = NULL;
	}
	for (int i = 0; i < map->capacity; i++) {
		if (map->keys[i] == NULL || map->values[i] == NULL) continue;
		int newHash = hash(map->keys[i]) % newCapacity;
		if (newHash < 0) newHash += newCapacity;
		while (newKeys[newHash] != NULL) {
			newHash = (newHash + 1) % newCapacity;
		}
		newKeys[newHash] = map->keys[i];
		newValues[newHash] = map->values[i];
	}
	free(map->keys);
	free(map->values);
	map->keys = newKeys;
	map->values = newValues;
	map->capacity = newCapacity;
}

IntHashMap new_IntHashMap() {
	IntHashMap this = (IntHashMap)malloc(sizeof(struct IntHashMap));
	if (this == NULL) return NULL;

	this->count = 0;
	this->capacity = HASHMAP_INIT_CAPACITY;
	this->keys = (int*)malloc(this->capacity * sizeof(int));
	this->values = (void**)malloc(this->capacity * sizeof(void*));
	if (this->keys == NULL || this->values == NULL) return NULL;
	for (int i = 0; i < this->capacity; i++) {
		this->keys[i] = -1;
		this->values[i] = NULL;
	}

	return this;
}

void IntHashMap_free(IntHashMap map) {
	free(map->keys);
	free(map->values);
	free(map);
}

void IntHashMap_insert(IntHashMap map, int key, void* value) {
	if (value == NULL) {
		printf("HashMap error: null key.\n");
		exit(EXIT_FAILURE);
	}
	if (map->count >= map->capacity / 2) resizeInt(map, map->capacity * 2);

	int hashVal = key % map->capacity;
	if (hashVal < 0) hashVal += map->capacity;
	while (map->keys[hashVal] != -1) {
		hashVal = (hashVal + 1) % map->capacity;
	}
	map->keys[hashVal] = key;
	map->values[hashVal] = value;
	map->count++;
}

void* IntHashMap_get(IntHashMap map, int key) {
	int hashVal = key % map->capacity;
	if (hashVal < 0) hashVal += map->capacity;
	int start = hashVal;
	if (start < 0) printf("key = %d, hash = %d, capacity = %d\n", key, start, map->capacity);
	while (map->keys[hashVal] != key) {
		hashVal = (hashVal + 1) % map->capacity;
		if (hashVal == start) {
			printf("HashMap error: key does not exist in table.\n");
			exit(EXIT_FAILURE);
		}
	}
	return map->values[hashVal];
}

void IntHashMap_remove(IntHashMap map, int key) {
	int hashVal = key % map->capacity;
	if (hashVal < 0) hashVal += map->capacity;
	int start = hashVal;
	while (map->keys[hashVal] != key) {
		hashVal = (hashVal + 1) % map->capacity;
		if (hashVal == start) return;
	}
	map->keys[hashVal] = -1;
	map->values[hashVal] = NULL;
	map->count--;
}

void resizeInt(IntHashMap map, int newCapacity) {
	int* newKeys = (int*)malloc(newCapacity * sizeof(int));
	void** newValues = (void**)malloc(newCapacity * sizeof(void*));
	if (newKeys == NULL || newValues == NULL) {
		printf("List error: could not resize HashMap to capacity %d\n", newCapacity);
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < newCapacity; i++) {
		newKeys[i] = -1;
		newValues[i] = NULL;
	}
	for (int i = 0; i < map->capacity; i++) {
		if (map->keys[i] == -1 || map->values[i] == NULL) continue;
		int newHash = map->keys[i] % newCapacity;
		if (newHash < 0) newHash += newCapacity;
		while (newKeys[newHash] != -1) {
			newHash = (newHash + 1) % newCapacity;
		}
		newKeys[newHash] = map->keys[i];
		newValues[newHash] = map->values[i];
	}
	free(map->keys);
	free(map->values);
	map->keys = newKeys;
	map->values = newValues;
	map->capacity = newCapacity;
}