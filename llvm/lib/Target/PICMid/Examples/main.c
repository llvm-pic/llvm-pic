#include <stdint.h>

int foo(int baa) {
    return baa++;
}

void main() {
    //declare
    uint16_t i, j, h;

    //initialize
    i = 2;
    j = 3;

    // integer operations
    h = i + j;
    h = i * j;
    h = i - j;
    h = i / j;
    h = i | j;
    h = i & j;
    h = i ^ j;
    h = i >> j;
    h = i << j; 
    h = i || j;
    h = i && j;
    h = i % j;

    // for loop

    for (int k = 0; k < 10; k++) {
       i += k; 
    } 

    if (i < 6) {
        h = 3; 
    }

    else {
        h = 4;
    } 

    h = foo(h);
}