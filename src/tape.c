#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "tape.h"

/* Possibly grow the array.
 * If current size is larger than current range, the array will grow to
 * an appropriate power of two.
 * The new space is initialized with blanks.
 * @param d self
 */
static void dynarr_grow(struct dynarr *d) {
    //test if we have to grow
    if (d->size <= d->range)
        return;

    int r = d->range;
    //grow in powers of two until we can fit new size
    do {
        r <<= 1;
    } while (d->size > r);

    d->range = r;
    d->data = realloc(d->data, r);
    //initialize array with blanks
    memset(d->data + d->size, BLANK, r - d->size);
}

/* Possibly shrink the array.
 * If current size is at most 1/4 of current range, the array will shrink
 * to an appropriate power of two.
 * @param d self
 */
static void dynarr_shrink(struct dynarr *d) {
    //test if we have to shrink
    if (d->size > d->range >> 2 || d->range == DYNARR_SIZE_MIN)
        return;

    int r = d->range;
    //shrink in powers of two until desired size is reached
    do {
        r >>= 1;
    } while (d->size <= d->range >> 2 && d->range > DYNARR_SIZE_MIN);

    d->range = r;
    d->data = realloc(d->data, r);
}

/* Write data to a specified position in the array.
 * The array is possibly resized.
 * @param d self
 * @param data the char to write
 * @param index the position to write to
 */
static void *dynarr_write(struct dynarr *d, char data, int index) {
    if (data == BLANK) {
        //no need to extend the array, since nothing important is written
        //only write data if something can be overwritten
        if (index < d->size) {
            *(data + index) = BLANK;
        }
        if (index == d->size - 1) {
            //the last important symbol is overwritten; the size changes
            //backtrack to last important index
            while (--index >= 0 && *(d->data + index) == BLANK)
                ; //note: index can get negative here; must be signed
            d->size = index + 1
            //check new size
            dynarr_shrink(d);
        }
    } else {
        //possibly extend array
        if (index >= d->size)
            d->size = index + 1;
        dynarr_grow(d);
            
        *(d->data + index) = data;
    }
}

#define DYNARR_INIT(d) \
    (d).size = 0; \
    (d).range = DYNARR_SIZE_MIN; \
    (d).data = malloc(DYNARR_SIZE_MIN); \
    memset((d).data, BLANK, DYNARR_SIZE_MIN)

/* Create an empty tape on the heap.
 * Both halves are initialized to minimum size, filled with blanks
 * @return new tape
 */
struct tape *tape_create_empty() {
    struct tape *t = malloc(sizeof(struct tape));

    DYNARR_INIT(t->l);
    DYNARR_INIT(t->r);

    return t;
}

/* Destroy tape.
 * @param t self
 */
void tape_destroy(struct tape *t) {
    free((t->l).data);
    free((t->r).data);

    free(t);
}

/* Write one char to specified position in tape.
 * Index can be any signed integer.
 * @param t self
 * @param data the char to write
 * @param index write position
 */
void tape_write(struct tape *t, char data, int index) {
    if (index >= 0)
        dynarr_write(&(t->r), data, index);
    else
        dynarr_write(&(t->l), data, ~index);
}

/* Put tape content in buffer.
 * Buffer will be null-terminated.
 * @param t self
 * @param buf buffer to write to
 */
void tape_sprint(struct tape *t, char *buf) {
    int i, j = 0;
    unsigned int size;
    char data;

    size = t->l.size;
    for (i = 0; i < size; i++) {
        data = *((t->l).data + size - i - 1);
        *(buf + j++) = data;
    }

    size = t->r.size;
    for (i = 0; i < size; i++) {
        data = *((t->r).data + i);
        *(buf + j++) = data;
    }

    *(buf + j) = '\0';
}
