#include <stdio.h>
#include <string.h>
#include <libpmemobj.h>
#include "layout.h"

int main(int argc, char ** argv) {
	// open our peristent memory file
	PMEMobjpool *pop = pmemobj_open(argv[1], LAYOUT_NAME);
	if (pop == NULL) {
		perror("pmemobj_open");
		return 1;
	}

	// retieve the persistent pointer and create a direct pointer
	PMEMoid root = pmemobj_root(pop, sizeof(struct my_root));
	struct my_root * rootp = pmemobj_direct(root);

	// verify the contents are the proper length and print
	if (rootp->len == strlen(rootp->buf))
		printf("%s\n", rootp->buf);

	// don't forget to close the pmem file
	pmemobj_close(pop);
	return 0;
}
