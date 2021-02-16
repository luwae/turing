#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynarr.h"

#define DYNARR_MIN_SIZE 8

/**
 * Create a new dynarray that is filled with blanks.
 * @return new dynarray
 */
struct dynarr *dynarr_new() {
    struct dynarr *d = malloc(sizeof(*d));

    d->size = 0;
    d->range = DYNARR_MIN_SIZE;
    d->data = malloc(DYNARR_MIN_SIZE);
    memset(d->data, BLANK, DYNARR_MIN_SIZE);

    return d;
}

/**
 * Free dynarray and associated memory.
 * @param d self
 */
void dynarr_free(struct dynarr *d) {
    free(d->data);
    free(d);
}

/**
 * Grow a dynarray so that a specific index can be accessed.
 * Fill the new space with blanks.
 * @param d self
 * @param index the index that is accessed
 */
static void dynarr_growto(struct dynarr *d, int index) {
    int newrange = d->range;
    while (index >= newrange)
        newrange <<= 1;

    d->data = realloc(d->data, newrange);
    memset(d->data + d->range, BLANK, newrange - d->range);

    d->range = newrange;
}

/**
 * Set a character in a dynarray.
 * Call succeeds for all index >= 0; dynarray might be resized.
 * @param d self
 * @param c the character to write
 * @param index where to write
 */
void dynarr_setchar(struct dynarr *d, char c, int index) {
    if (c == BLANK) {
        //the character is a blank. This means the dynarr cannot grow.
        //if the blank is outside our current size, nothing is to be done.
        if (index < d->size) {
            *(d->data + index) = c;
            if (index == d->size - 1) {
                //reduce size because last element was cleared
                while(d->size > 0 && *(d->data + d->size - 1) == BLANK) {
                    d->size--;
                }
            } 
        }
    } else {
        if (index >= d->range)
            dynarr_growto(d, index);
        *(d->data + index) = c;
        if (index >= d->size)
            d->size = index + 1;
    }
}

/**
 * Get char at given position in the dynarray.
 * Returns blank if the index is outside the current range.
 * @param d self
 * @param index where to look
 * @return the character at this position
 */
char dynarr_getchar(struct dynarr *d, int index) {
    if (index >= d->range)
        return BLANK;
    return *(d->data + index);
}

/**
 * Print contents of dynarray to string, until last non-blank character.
 * Null-terminated.
 * @param d self
 * @param s write location
 */
void dynarr_sprint(struct dynarr *d, char *s) {
    memcpy(s, d->data, d->size);
    *(s + d->size) = '\0';
}

/* test */
#ifdef DEBUG
static void *dynarr_print(struct dynarr *d) {
    char s[d->size + 1];
    dynarr_sprint(d, s);
    printf("Dynarr\n  size=%d, range=%d\n  content=%s\n", d->size, d->range, s);
}

int main() {
    struct dynarr *d = dynarr_new();
    dynarr_print(d);
    dynarr_setchar(d, 'A', 3);
    dynarr_print(d);
    dynarr_setchar(d, 'B', 50);
    dynarr_print(d);
    for (int i = 0; i < d->size; i++)
        printf("%c ", dynarr_getchar(d, i));
    printf("\n");
    dynarr_setchar(d, BLANK, 50);
    dynarr_print(d);
    dynarr_setchar(d, BLANK, 3);
    dynarr_print(d);
    dynarr_free(d);
}
#endif
