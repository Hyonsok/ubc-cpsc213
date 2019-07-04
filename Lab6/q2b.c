#include "stdio.h"
#include "stdlib.h"

int x[8] = {1,2,3,0xffffffff,0xfffffffe,0,184,340057058};
int y[8];

int f(int* x){
    int i = 0;
    int b = *x;
    int c = 0x80000000;

    while(b != 0) {
        int d = b;
        d = d&0x80000000;
        if (d != 0) {
            i++;
        }
        b = b << 1;
    }
    return i;
}

int main (int arc,char* argv[]) {
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

