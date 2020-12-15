#include <stdio.h>

#include "tape.h"

int main() {
    struct tapeFin *t = getFiniteTape(100);
    printf("%d %d\n", t->length, t->numBytes);
    
    setBit(t, 3);
    
    dumpTape(t);
    destroyFiniteTape(t);
    
    return 0;
}
