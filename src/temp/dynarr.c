#include <stdio.h>
#include <stdlib.h>

#include <dynarr.h>

#define DYNARR_RANGE(d) ((d)->nblocks << BLOCK_SHIFT)
#define IN_RANGE(d, index) (index) < DYNARR_RANGE(d)

/* Get a block from heap, filled with blanks.
 * @return new block
 */
static struct block *block_get() {
    struct block *b = malloc(sizeof(struct block));

    memset(b, BLANK, BLOCK_SIZE);
    return b;
}

/* Free a block obtained via block_get().
 * These functions exist to keep memory allocation abstract.
 * A further possibility would be a slab allocator instead of malloc.
 * @param b self
 */
static void block_free(struct block *b) {
    free(b);
}

/* Initialize a dynamic array.
 * The array contains 1 block of size BLOCK_SIZE.
 * @param d self
 */
void dynarr_init(struct dynarr *d) {
    d->size = 0;
    d->nblocks = 1;
    d->blocks = malloc(sizeof(struct block *));
    *(d->blocks) = block_get();
}

/* Free all memory associated with a dynamic array.
 * Does not free the array itself: usually not allocated with malloc.
 * @param d self
 */
void dynarr_free(struct dynarr *d) {
    int i;

    for (i = 0; i < d->nblocks; i++) {
        block_free(*(d->blocks + i));
    }
    free(d->blocks);
}

#define REALLOC_NUM_BLOCKS(d, n) \
    (d)->blocks = realloc((d)->blocks, n * sizeof(struct block *))

/* Grow a dynamic array to specified indexing size.
 * Only call this function if a need for growing is certain.
 * @param d self
 * @param index the index to be accessed
 */
static void dynarr_grow(struct dynarr *d, int index) {
    int nreq = (index >> BLOCK_SHIFT) + 1;
    int ncur = d->nblocks;

    REALLOC_NUM_BLOCKS(d, nreq);

    while (ncur < nreq)
        *(d->blocks + ncur++) = block_get();
    d->nblocks = nreq;
}

/* Possibly shrink a dynamic array if more than one block is empty.
 * This function can be called always, unlike dynarr_grow().
 * @param d self
 */
static void dynarr_tryshrink(struct dynarr *d) {
    if (size > (d->nblocks - 2) << BLOCK_SHIFT)
        return;

    int diff = DYNARR_RANGE(d) - size; //how much empty space in array
    int nreq = d->nblocks; //how many after shrink
    int i;

    do {
        nreq--;
        diff -= BLOCK_SIZE;
    } while (diff > BLOCK_SIZE); //shrink until only one empty block

    for (i = nreq; i < d->nblocks; i++)
        block_free(*(d->blocks + i));

    REALLOC_NUM_BLOCKS(d, nreq);
    d->nblocks = nreq;
}

#define WRITE_TO_BLOCK(d, nblock, nbyte, data) \
    *(*((d)->blocks + nblock) + nbyte) = data;

#define DATA_AT_BLOCK(d, nblock, nbyte) \
    *(*((d)->blocks + nblock) + nbyte)

/* Write to dynamic array at an arbitrary index, possibly changing size.
 * @param d self
 * @param data the char to write
 * @param index where to write
 */
void dynarr_write(struct dynarr *d, char data, int index) {
    int nblock = index >> BLOCK_SHIFT;
    int nbyte = index % BLOCK_SIZE;

    if (data == BLANK) {
        if (index < d->size)
            //only write of something might be overwritten
            WRITE_TO_BLOCK(d, nblock, nbyte, data);
        if (index == size - 1) {
            //last important entry was overwritten; need to change size
            //index can get negative here, needs to be signed
            while(index-- >= 0 && DATA_AT_BLOCK(d, nblock, nbyte) == BLANK)
                ;
            d->size = index + 1;
            dynarr_tryshrink(d);
        }
    } else {
        if (!IN_RANGE(d, index))
            dynarr_grow(d, index);
        if (index >= d->size)
            d->size = index + 1;
        WRITE_TO_BLOCK(d, nblock, nbyte, data);
    }
}

/* Read from dynamic array.
 * @param d self
 * @param index where to read
 * @return char at index
 */
char dynarr_read(struct dynarr *d, int index) {
    int nblock = index >> BLOCK_SHIFT;
    int nbyte = index % BLOCK_SIZE;

    return DATA_AT_BLOCK(d, nblock, nbyte);
}

/* Print null-terminated array contents to buffer.
 * @param d self
 * @param buf the buffer to write to
 * @return number of bytes written, excluding null byte
 */
int dynarr_sprint(struct dynarr *d, char *buf) {
    int nbyte = 0;
    int i = 0;
    struct block *b = *(d->blocks);

    while (i < d->size) {
        *(buf + i++) = *(b + nbyte++);
        if (nbyte == BLOCK_SIZE) {
            nbyte = 0;
            b++;
        }
    }

    *(buf + j) = '\0';
    return d->size;
}
