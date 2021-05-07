#include <stdio.h>
#include <string.h>
#include <libpmemobj.h>
#include "layout.h"

int main(int argc, char ** argv) {
	// open our peristent memory file
	PMEMobjpool *pop = pmemobj_open(argv[1], POBJ_LAYOUT_NAME(string_store));
	if (pop == NULL) {
		perror("pmemobj_open");
		return 1;
	}

	// retieve the persistent pointer and create a direct pointer
	// PMEMoid root = pmemobj_root(pop, sizeof(struct my_root));
	// struct my_root * rootp = pmemobj_direct(root);

	// same using declared layout
	TOID(struct my_root) root = POBJ_ROOT(pop, struct my_root);

	// Since the buffer was written with a transaction, we do not need to verify it's integrity
	//if (rootp->len == strlen(rootp->buf))
	printf("%s\n", D_RO(root)->buf);

	// don't forget to close the pmem file
	pmemobj_close(pop);
	return 0;
}
