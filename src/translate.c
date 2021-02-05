#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "arena.h"

#define ASCII_TABLE_SIZE 128
#define MISS_CHAR 0xff

uint8_t *translator_create(char *chars) {
    uint8_t *t = aalloc(ASCII_TABLE_SIZE);
    memset(t, MISS_CHAR, ASCII_TABLE_SIZE);
    
    char c;
    uint8_t count = 0;
    while (c = *(chars++))
        *(t + c) = count++;
    
    return t;
}

/* test */
#ifdef DEBUG
#define ROW_LEN 8

void translator_print(uint8_t *t) {
    for (int i = 0; i < ASCII_TABLE_SIZE; i++) {
        printf("%3i ", *(t + i));
        if (i % ROW_LEN == ROW_LEN - 1)
            printf("\n");
    }
}

int main() {
    ainit();
    uint8_t *t = translator_create("_01ab");
    translator_print(t);
    afree();
}
#endif
