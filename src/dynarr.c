#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynarr.h"

#define DYNARR_MIN_SIZE 8

/**
 * Create a new dynarray.
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
 * Free dynarray.
 * @param d self
 */
void dynarr_free(struct dynarr *d) {
    free(d->data);
    free(d);
}

/**
 * Grow a dynarray so that a specific index can be accessed.
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

void dynarr_setchar(struct dynarr *d, char c, int index) {
    if (char == BLANK && index < d->size)
        *(d->data + index) = c;
        if (index == size - 1) {
            //reduce size because last element was cleared
            //TODO
            do {
                d->size--;
            } while (d->size >= 0 && *(d->data + d->size) == BLANK);
            if (d->size == 0)d->size++;
        }
    else {
        if (index >= d->size)
            dynarr_growto(index);
    }
}
