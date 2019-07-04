#include "stdio.h"
#include "stdlib.h"

int a[4] = {0,0,0,0};
int foo(int q, int p, int r){
    void* jt[] = {&&L330,&&L384, &&L334, &&L384, &&L33c, &&L384, &&L354, &&L384, &&L36c};
    if (q < 10 || q > 18){
        goto L6;
    }
    goto *jt[q-10];

L330:
    r += p;
    goto L7;

L334:
    r = -r + p;
    goto L7;

L33c:
    if (r > p) {
        r = 1;
        goto L7;
    } else {
        r = 0;
        goto L7;
    }

L354:
    if (p > r) {
        r = 1;
        goto L7;
    } else {
        r = 0;
        goto L7;
    }

L36c:
    if (r == p) {
        r = 1;
        goto L7;
    } else {
        r = 0;
        goto L7;
    }
L384:
    r = 0;
    goto L7;
L6:
    r = 0;
    goto L7;

L7:
    return r;
}

int main (int argc, char** argv[]) {
    a[3] = foo(a[0],a[1],a[2]);
}