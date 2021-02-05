#include <stdlib.h>
#include <stdio.h>

#define INIT_SIZE 4096

char *arena;
size_t asize = 0;
size_t aoff = 0;

void ainit() {
    arena = malloc(asize = INIT_SIZE);
}

void *aalloc(size_t size) {
    if (asize - aoff < size)
        return NULL;
        
    void *p = arena + aoff;
    aoff += size;
    return p;
}

void afree() {
    free(arena);
}
