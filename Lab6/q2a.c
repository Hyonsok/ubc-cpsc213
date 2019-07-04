#include "stdio.h"
#include "stdlib.h"

int a[10] = {0,0,0,0,0,0,0,0,0,0}

void foo(int arg0, int arg1) {
    int temp;
    arg0 = temp[arg1]+ arg0;

}

void foo(int arg0, int arg1) {
    int i = a[arg1];
    a[arg1] = i + arg0;
}
void foz() {
    int p = 3;
    int q = 4;
    foo(p,q);
    p = 3;
    q = 4;
    foo(p,q);
}

int main(int argc, char* argv[]) {
    foz();
    printf("%d\n",a[0]);
    printf("%d\n",a[1]);
    printf("%d\n",a[2]);
    printf("%d\n",a[3]);
    printf("%d\n",a[4]);
    printf("%d\n",a[5]);
    printf("%d\n",a[6]);
    printf("%d\n",a[7]);
    printf("%d\n",a[8]);
    printf("%d\n",a[9]);

}