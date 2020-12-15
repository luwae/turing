#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define DEBUG

#define DYNARR_SIZE_MIN 8

/* A resizable array containing 8 bit unsigned integers.
 */
struct dynarr {
    unsigned int size; //current size (last used index is size - 1)
    unsigned int range; //current maximal size, always a power of two
    uint8_t *data;
};

/* A tape ranging from -infinity to infinity.
 */
struct tape {
    struct dynarr l; //left half, containing (-infinity..-1]
    struct dynarr r; //right half, containing [0..infinity)
};

/* Grow dynamic array to a minimum indexing size.
 * The new size will be index+1 rounded up to the next power of 2.
 * @param d self
 * @param index the index that should be accessible after resize
 */
static void *dynarr_growto(struct dynarr *d, int index) {
    int p = 1;

    while (p <= index)
        p <<= 1;

#ifdef DEBUG
    printf("DEBUG: reallocate dynarr %p to size %d\n", d, p);
#endif
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

#ifdef DEBUG
    printf("DEBUG: call tape_write_string\n");
#endif

    while (c = *(s + i)) {
#ifdef DEBUG
        printf("DEBUG: write char %c (index=%d) (uint8=%d) at %d\n", c, c, *(sym2num + c), i);
#endif
        dynarr_write(d, *(sym2num + c), i++);
    }
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

/* Print contents of tape.
 * @param t self
 */
void tape_dump(struct tape *t, char *num2sym) {
    int i;
    unsigned int size;
    uint8_t data;

#ifdef DEBUG
    printf("DEBUG: call tape_dump\n");
#endif
    size = t->l.size;
    for (i = 0; i < size; i++) {
        data = *((t->l).data + size - i - 1);
        printf("%c", *(num2sym + data));
    }

    size = t->r.size;
    for (i = 0; i < size; i++) {
        data = *((t->r).data + i);
        printf("%c", *(num2sym + data));
    }

    printf("\n");
}

#ifdef DEBUG
int main() {
    struct tape *t = tape_create_empty();
    uint8_t sym2num[] = {
        0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
        0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
        0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
        1   ,2   ,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
        0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
        0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0   ,
        0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
        0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
    };

    char *num2sym = "_01";
    
    tape_write_string(t, "0100101_01___1", sym2num);
    tape_write(t, 0, 13);
    tape_write(t, 2, -5);
    tape_dump(t, num2sym);

    tape_destroy(t);
    return 0;
}
#endif
