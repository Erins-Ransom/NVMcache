#include <unistd.h>
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include <time.h>
#include <sys/time.h>


int main(int argc, char *argv[])
{
	struct timeval start, end;
	double diff_t;

	gettimeofday (&start, NULL);

	if (argc == 2){
		system(argv[1]);
	} else {
		printf("Usage ./timer.o [executable]\n");
		exit(-1);
	}
	//system("./foo.o");

	gettimeofday (&end, NULL);
	diff_t = (((end.tv_sec - start.tv_sec)*1000000L
	+end.tv_usec) - start.tv_usec) / (1000000.0);
	printf("Execution time: %f \n", diff_t);

}
