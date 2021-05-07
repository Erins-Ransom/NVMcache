#include <stdio.h>
#include <libpmemobj.h>


POBJ_LAYOUT_BEGIN(rect_calc);
        POBJ_LAYOUT_ROOT(rect_calc, struct my_other_root);
        POBJ_LAYOUT_ROOT(rect_calc, struct rectangle);
POBJ_LAYOUT_END(rect_calc);


struct rectangle { 
        int a;
        int b;
};

struct my_other_root {
        TOID(struct rectangle) rect;
};


int area_calc(const TOID(struct rectangle) rect) {
	return D_RO(rect)->a * D_RO(rect)->b;
}

void rectangle_modify(PMEMobjpool *pop, TOID(struct rectangle) rect, int new_a, int new_b) {
	TX_BEGIN(pop) {
		// TX_SET(rect, a, new_a);
		// TX_SET(rect, b, new_b);

		// more efficient way
		TX_ADD(rect);
		D_RW(rect)->a = new_a;
		D_RW(rect)->b = new_b;
	} TX_END
}



int main(int argc, char ** argv) {

	PMEMobjpool *pop = pmemobj_create("pmem", POBJ_LAYOUT_NAME(rect_calc), PMEMOBJ_MIN_POOL, 0666);
	if (pop == NULL) {
		perror("pmemobj_create");
		return 1;
	}

	TOID(struct my_other_root) root = POBJ_ROOT(pop, struct my_other_root);
	// allocate and initalize the sturcture that root points to
	TX_BEGIN(pop) {
		TX_ADD(root); // designates the pointer root as part of the transaction
		TOID(struct rectangle) rect = TX_NEW(struct rectangle);
		D_RW(rect)->a = 5;
		D_RW(rect)->b = 10;
		D_RW(root)->rect = rect;
	} TX_END

	// read value
	int p = area_calc(D_RO(root)->rect);
	printf("Area of rectangle is %d\n", p);

	// adjust the values
	rectangle_modify(pop, D_RW(root)->rect, 2, 20);

	// read new values
	p = area_calc(D_RO(root)->rect);
        printf("Area of rectangle is %d\n", p);


	// deallocate the pmem
	TX_BEGIN(pop) {
		TX_ADD(root);
		TX_FREE(D_RW(root)->rect);
		D_RW(root)->rect = TOID_NULL(struct rectangle);
	} TX_END

	return 0;
}
