/**
 * Translator from chars to integers for fast turing machine traversal.
 * @author LuWae
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "translate.h"

#define ASCII_SIZE 128

/**
 * Create an array that maps chars to integers, starting at 0.
 * All translations are set to invalid (TRANSLATOR_MISS), except for chars.
 * @param chars the chars to map
 * @return new translator
 */
uint8_t *translator_new(char *chars) {
    uint8_t *t = malloc(ASCII_SIZE);
    memset(t, TRANSLATOR_MISS, ASCII_SIZE);
    
    char c;
    uint8_t count = 0;
    while ((c = *(chars++)))
        *(t + c) = count++;
    
    return t;
}

/**
 * Free translator.
 * @param t self
 */
void translator_free(uint8_t *t) {
    free(t);
}

/* test */
#ifdef DEBUG
#define ROW_LEN 8

static void translator_print(uint8_t *t) {
    for (int i = 0; i < ASCII_SIZE; i++) {
        uint8_t translated = *(t + i);
        if (translated == TRANSLATOR_MISS)
            printf("  ? ");
        else
            printf("%3i ", translated);
            
        if (i % ROW_LEN == ROW_LEN - 1)
            printf("\n");
    }
}

int main() {
    uint8_t *t = translator_new("_01ab");
    translator_print(t);
    translator_free(t);
}
#endif
