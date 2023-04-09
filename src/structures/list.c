#pragma warning( disable: 6386 )

#include <stdlib.h>
#include <stdio.h>
#include "list.h"

void List_resizedouble(List list);

List new_List() {
	List this = (List)malloc(sizeof(struct List));
	if (this == NULL) return NULL;

	this->capacity = 10;
	this->count = 0;
	this->data = (void**)malloc(this->capacity * sizeof(void*));
	return this;
}

void List_free(List list) {
	free(list->data);
	free(list);
}

void* List_get(List list, int index) {
	if (index >= list->count || index < 0) {
		printf("List error: index out of bounds\n");
		exit(EXIT_FAILURE);
	}
	if (list->data[index] == NULL) {
		printf("List error: null pointer\n");
		exit(EXIT_FAILURE);
	}
	return list->data[index];
} 

void List_insert(List list, void* data, int index) {
	if (list->count >= list->capacity) {
		List_resizedouble(list);
	}
	if (index >= list->count || index < 0) {
		printf("List error: index out of bounds\n");
		exit(EXIT_FAILURE);
	}
	for (int i = list->count - 1; i >= index; i--) {
		list->data[i + 1] = list->data[i];
		list->data[i] = NULL;
	}
	list->data[index] = data;
	list->count++;
}

void List_remove(List list, int index) {
	if (index >= list->count || index < 0) {
		printf("List error: index out of bounds\n");
		exit(EXIT_FAILURE);
	}
	list->data[index] = NULL;
}

void List_set(List list, void* data, int index) {
	if (index > list->count || index < 0) {
		printf("List error: index out of bounds\n");
		exit(EXIT_FAILURE);
	}
	else if (index == list->count) {
		List_add(list, data);
		return;
	}
	list->data[index] = data;
}

void List_add(List list, void* data) {
	if (list->count >= list->capacity) {
		List_resizedouble(list);
	}
	list->data[list->count] = data;
	list->count++;
}

void* List_getfirst(List list) {
	if (list->count == 0) {
		printf("List error: list is empty\n");
		exit(EXIT_FAILURE);
	}
	return list->data[0];
}

void* List_getlast(List list) {
	if (list->count == 0) {
		printf("List error: list is empty\n");
		exit(EXIT_FAILURE);
	}
	return list->data[list->count - 1];
}

void List_resizedouble(List list) {
	list->capacity = list->capacity * 2;
	void** newData = (void**)malloc(list->capacity * sizeof(void*));
	if (newData == NULL) {
		printf("List error: could not resize list to capacity %d\n", list->capacity);
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < list->count; i++) {
		newData[i] = list->data[i];
	}
	free(list->data);
	list->data = newData;
}