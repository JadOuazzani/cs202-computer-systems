#include <stdio.h>

/*
	Exercise 1

    To learn:
      Compilation for debuging
      start gdb
      use:
         breakpoints
         next, step, continue
         [ advance, finish ]
         print
*/

int bar(int i) {
	return (((i * 63) & 51) + 37) ^ 5;
}

void foo(void) {
	puts("Computing...");

	for(int i = 0; i <= 5; ++i) {
		int d = bar(i);
		printf("d%d is %s\n", i, d&1 ? "odd" : "even");
	}

	int d = bar(17);
	printf("d%d is %s\n", 17, d&1 ? "odd" : "even");

	puts("Done !");
}


int main(void)
{
	puts("Hello world!");
	foo();
	return 0;
}
