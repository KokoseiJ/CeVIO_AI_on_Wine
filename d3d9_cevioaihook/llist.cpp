#include "llist.h"

Linkedlist* llist_put(Linkedlist* list, const char* key, void* value, size_t size) {
	if (list == NULL) {
		list = (Linkedlist*)calloc(1, sizeof(Linkedlist));
		if (list == NULL) return NULL;
		*list = { key, value, size, NULL };
		return list;
	}

	if (strcmp(key, list->key) == 0) {
		list->value = value;
		list->size = size;
	}
	else {
		list->next = llist_put(list->next, key, value, size);
		list->size = size;
	}

	return list;
}

Linkedlist* llist_get(Linkedlist* list, const char* key) {
	if (list == NULL) return NULL;

	if (strcmp(key, list->key) == 0) {
		return list;
	}
	else return llist_get(list->next, key);
}

void llist_free(Linkedlist* list) {
	if (list != NULL) {
		llist_free(list->next);
		free(list);
	}
	return;
}
