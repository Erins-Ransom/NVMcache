#include <stdio.h>
#include <string.h>
#include <libpmemobj.h>

POBJ_LAYOUT_BEGIN(string_store);
POBJ_LAYOUT_ROOT(string_store, struct my_root);
POBJ_LAYOUT_END(string_store);


#define LAYOUT_NAME "test_0"
#define MAX_BUF_LEN 10

// This layout is intended to store a single string
struct my_root {
	size_t len; // string length
	char buf[MAX_BUF_LEN]; // buffer for contents
};


