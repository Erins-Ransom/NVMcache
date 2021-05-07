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
	// PMEMobjpool *pop = pmemobj_create(argv[1], LAYOUT_NAME, PMEMOBJ_MIN_POOL, 0666);
	// or, using the declared layout in layout.h
	PMEMobjpool *pop = pmemobj_create(argv[1], POBJ_LAYOUT_NAME(string_store), PMEMOBJ_MIN_POOL, 0666);
	if (pop == NULL) {
		perror("pmemobj_create");
		return 1;
	}

	// Create a persistent pointer based on our layout and then create a direct 
	// pointer to the structure within
	//PMEMoid root = pmemobj_root(pop, sizeof(struct my_root));
	//struct my_root * rootp = pmemobj_direct(root);

	// or, using the layout in layout.h we can instead do
	TOID(struct my_root) root = POBJ_ROOT(pop, struct my_root);

	// read a string into volatile memory
	char buf[MAX_BUF_LEN];
	scanf("%9s", buf);

	// This time we will use a transaction to write to pmem
//	TX_BEGIN(pop) {
//		pmemobj_tx_add_range(root, 0, sizeof(struct my_root));
//		memcpy(rootp->buf, buf, strlen(buf));
//	} TX_ONCOMMIT {
//		// optional, code to be executed after tx is commited
//	} TX_ONABORT {
//		// optional, code to be executed if tx aborts
//	} TX_FINALLY {
//		// optional, code to be executed after abort or commit
//	} TX_END

	// Alternate 
//	TX_BEGIN(pop) {
//                pmemobj_tx_add_range_direct(rootp->buf, MAX_BUF_LEN);
//                memcpy(rootp->buf, buf, strlen(buf));
//        } TX_END

	// using declared layout
	TX_BEGIN(pop) {
                TX_MEMCPY(D_RW(root)->buf, buf, strlen(buf));
        } TX_END
	
	// without transactions
	// rootp->len = strlen(buf);
	// pmemobj_persist(pop, &rootp->len, sizeof(rootp->len));
	// pmemobj_memcpy_persist(pop, rootp->buf, buf, rootp->len);

	//don't forget to close the persistent memory file
	pmemobj_close(pop);
	return 0;
}
