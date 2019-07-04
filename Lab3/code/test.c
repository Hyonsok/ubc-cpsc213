#include "stdio.h"
#include "stdlib.h"



void update() {
    int a[10] = {0,1,2,3,4,5,6,7,8,9};
}

int main (int argc, char* argv[]) {
    char* ep;
    array = malloc(3 * sizeof(int));

    if (argc != 4) {
        printf("invalid number of arguments, takes 3 numbers\n");
        return -1;
    }

    for (int i=0; i<3; i++) {
        array[i] = strtol (argv[i+1], &ep, 10);

        if (*ep) {
            printf ("Argument %d is not a number\n", i);
            return -1;
        }
    }

    update();


    printf("t: %d\n", t);

    for (int i=0; i<3; i++)
        printf("array[%d]: %d\n", i, array[i]);

    return 0;

}
