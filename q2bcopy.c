#include <stdlib.h>
#include <stdio.h>
int x[8];
int y[8];

int f(int* a) {
	int j = 0;
	int b = *a;
	int z = 0x80000000;
	int c;
	while (b != 0) {
		c = b;
		c = c & z;
		if (c != 0) {
			j++;
		}
		b = b << 1;
	}
	return j;
}

int main(int ac, char** av) {
	for (int i = 7; i >= 0; i--) {
    		y[i] = f(&x[i]);
    	}
    	printf("%d\n", x[0]);
    	printf("%d\n", x[1]);
    	printf("%d\n", x[2]);
    	printf("%d\n", x[3]);
    	printf("%d\n", x[4]);
    	printf("%d\n", x[5]);
    	printf("%d\n", x[6]);
    	printf("%d\n", x[7]);
    	printf("%d\n", y[0]);
    	printf("%d\n", y[1]);
    	printf("%d\n", y[2]);
    	printf("%d\n", y[3]);
    	printf("%d\n", y[4]);
    	printf("%d\n", y[5]);
    	printf("%d\n", y[6]);
    	printf("%d\n", y[7]);
}


