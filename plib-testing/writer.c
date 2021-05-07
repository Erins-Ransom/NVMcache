#include <stdio.h>
#include <string.h>
#include <libpmemobj.h>
#include "layout.h"

int main(int argc, char ** argv) {

	if (argc <= 1) {
		printf("usage: ./write [pmem file name]\n");
		return 0;
	}

	// This call allocates our persistent memory
	PMEMobjpool *pop = pmemobj_create(argv[1], LAYOUT_NAME, PMEMOBJ_MIN_POOL, 0666);
	if (pop == NULL) {
		perror("pmemobj_create");
		return 1;
	}

	// Create a persistent pointer based on our layout and then create a direct 
	// pointer to the structure within
	PMEMoid root = pmemobj_root(pop, sizeof(struct my_root));
	struct my_root * rootp = pmemobj_direct(root);

	// read a string into volatile memory
	char buf[MAX_BUF_LEN];
	scanf("%9s", buf);

	// first we directly update rootp->len and then persist it
	// then we persist the contents of the string
	rootp->len = strlen(buf);
	pmemobj_persist(pop, &rootp->len, sizeof(rootp->len));
	pmemobj_memcpy_persist(pop, rootp->buf, buf, rootp->len);

	// don't forget to close the persistent memory file
	pmemobj_close(pop);
	return 0;
}
