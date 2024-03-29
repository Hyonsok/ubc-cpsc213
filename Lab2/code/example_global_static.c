#include "stdio.h"
#include "stdlib.h"

int a;
int b[10];
void foo () {
    b[a] = a;
    a++;
}


int main (int argc, char* argv[]) {
    char* ep;
    
    if (argc != 2) {
        printf("invalid number of arguments, takes 1 number\n");
        return -1;
    }
    
    a = strtol (argv[1], &ep, 10);
    if (*ep) {
        printf ("Argument is not a number\n");
        return -1;
    }

    if (a>=10||a<0) {
        printf("invalid input\n");
        return -1;
    }
    
    printf ("\nbefore foo()\n");
    
    printf("a: %d, b[%d]: %d\n", a, a, b[a]);
    
    foo();
    
    printf ("\nafter foo()\n");
    
    printf("a: %d, b[%d]: %d, b[%d]: %d\n", a, a, b[a], a-1, b[a-1]);
    
    return 0;
    
}
