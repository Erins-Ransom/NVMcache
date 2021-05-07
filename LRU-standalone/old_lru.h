#ifndef OLD_LRU_H
#define OLD_LRU_H

#include <stdlib.h>

typedef struct list_item {

	struct list_item * next;
	struct list_item * prev;
	uint64_t key;
	uint64_t size; 

} item;

typdef struct double_llist {
	item * first; 
	item * last;
	uint64_t total_size; 
	uint64_t size_lim;
} list;

list * new_list(uint64_t size_lim);
void add_item(list * List, uint64_t key, uint64_t size); 
int boost_item(list * List, uint64_t key);
void remove_item(list * List, uint64_t key);

#endif
