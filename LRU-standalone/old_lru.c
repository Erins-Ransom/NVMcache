#include "old_lru.h"


list * new_list(uint64_t size_lim) {
	list * L = malloc(sizeof(list));
	L->first = NULL;
	L->last = NULL;
	L->total_size = 0;
	L->size_lim = size_lim;
	return L;
}

void evict(list * L) {
	if (!L->last) return;
	item * it = L->last;
	L->total_size -= it->size;
	L->last = it->prev;
	free(it);
}

void add_item(list * L, uint64_t key, uint64_t size) {

	while (L->total_size + size > L->size_lim) 
		evict(L);
	item * it = malloc(sizeof(item));
	it->key = key;
	it->size = size;
	it->next = L->first;
	it->prev = NULL;
	if (L->first) 
		L->first->prev = it;
	L->first = it;
	return;
}

int boost_item(list * L, uint64_t key) {
	item * it = L->first;
	while (it && it->key != key)
		it = it->next;
	if (!it) 
		return 1;
	if (it->prev)
		it->prev->next = it->next;
	if (it->next)
		it->next->prev = it->prev;
	it->next = L->first;
	it->prev = NULL;
	return 0;
}

void remove_item(list * L, uint64_t key) {
	item * it = L->first;
        while (it && it->key != key)
                it = it->next;
        if (!it)
                return;
        if (it->prev)
                it->prev->next = it->next;
        if (it->next)
                it->next->prev = it->prev;
	free(it);
	return;
}
