#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "tape.h"

#define DYNARR_SIZE_MIN 8

/* Grow dynamic array to a minimum indexing size.
 * The new size will be index+1 rounded up to the next power of 2.
 * @param d self
 * @param index the index that should be accessible after resize
 */
static void *dynarr_growto(struct dynarr *d, int index) {
    int p = 1;

    while (p <= index)
        p <<= 1;

    d->data = realloc(d->data, p);
    d->range = p;
}

/* Write data to a specified position in the array.
 * The array is possibly resized.
 * @param d self
 * @param data the number to write
 * @param index the position to write to
 */
static void *dynarr_write(struct dynarr *d, uint8_t data, int index) {
    if (index >= d->range)
        dynarr_growto(d, index);

    *(d->data + index) = data;

    //update size
    if (data == 0) {
        //blank, find last non blank
        while (--index >= 0)
            if (*(d->data + index))
                break;
    }
    d->size = index + 1;
}

/* Create an empty tape on the heap.
 * Both halves are initialized to minimum size, filled with blanks
 * @return new tape
 */
struct tape *tape_create_empty() {
    struct tape *t = malloc(sizeof(struct tape));

    (t->l).size = 0;
    (t->l).range = DYNARR_SIZE_MIN;
    (t->l).data = malloc(DYNARR_SIZE_MIN);
    (t->r).size = 0;
    (t->r).range = DYNARR_SIZE_MIN;
    (t->r).data = malloc(DYNARR_SIZE_MIN);

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

/* Write string to tape starting at postion 0.
 * @param t self
 * @param s the string to write
 * @param sym2num converter table from char to uint8
 */
void tape_write_string(struct tape *t, char *s, uint8_t *sym2num) {
    char c;
    int i = 0;
    struct dynarr *d = &(t->r);

    while (c = *(s + i))
        dynarr_write(d, *(sym2num + c), i++);
}

/* Write one data byte to specified position.
 * Index can be any signed integer.
 * @param t self
 * @param data the byte to write
 * @param index write position
 */
void tape_write(struct tape *t, uint8_t data, int index) {
    if (index >= 0)
        dynarr_write(&(t->r), data, index);
    else
        dynarr_write(&(t->l), data, ~index);
}

/* Put tape content in buffer.
 * @param t self
 * @param buf buffer to write to
 * @param num2sym translation table from uint8 to char
 */
void tape_sprint(struct tape *t, char *buf, char *num2sym) {
    int i, j = 0;
    unsigned int size;
    uint8_t data;

    size = t->l.size;
    for (i = 0; i < size; i++) {
        data = *((t->l).data + size - i - 1);
        *(buf + j++) = *(num2sym + data);
    }

    size = t->r.size;
    for (i = 0; i < size; i++) {
        data = *((t->r).data + i);
        *(buf + j++) = *(num2sym + data);
    }

    *(buf + j) = '\0';
}
