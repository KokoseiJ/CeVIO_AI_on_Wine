#ifndef INCL_LLIST_H
#define INCL_LLIST_H

#include <stdlib.h>
#include <string.h>

typedef struct linkedlist Linkedlist;

typedef struct linkedlist {
	const char* key;
	void* value;
	size_t size;
	Linkedlist* next;
} Linkedlist;

Linkedlist* llist_put(Linkedlist* list, const char* key, void* value, size_t size);
Linkedlist* llist_get(Linkedlist* list, const char* key);
void llist_free(Linkedlist* list);

#endif
