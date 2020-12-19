#include <stdio.h>
#include <stdlib.h>

#define BLANK '_'

#define BLOCK_SIZE 256
#define BLOCK_SHIFT 8

#define DYNARR_RANGE(d) (d)->nblocks << BLOCK_SHIFT
#define IN_RANGE(d, index) (index) < (DYNARR_RANGE(d))

struct block {
    char data[BLOCK_SIZE];
};

struct dynarr {
    unsigned int size;
    unsigned int nblocks;
    struct **blocks;
}

struct tape {
    struct dynarr l;
    struct dynarr r;
};

static struct block *block_get() {
    struct block *b = malloc(sizeof(struct block));

    memset(b, BLANK, BLOCK_SIZE);
    return b;
}

static void block_free(struct block *b) {
    free(b);
}

static void dynarr_init(struct dynarr *d) {
    d->size = 0;
    d->nblocks = 1;
    d->blocks = malloc(sizeof(struct block *));
    *(d->blocks) = block_get();
}

static void dynarr_free(struct dynarr *d) {
    int i;

    for (i = 0; i < d->nblocks; i++) {
        free(*(d->blocks + i));
    }
    free(d->blocks);
}

static void dynarr_grow(struct dynarr *d, int index) {
    int nreq = (index >> BLOCK_SHIFT) + 1;
    int ncur = d->nblocks;

    d->blocks = realloc(d->blocks, nreq * sizeof(struct block *));

    while (ncur < nreq) {
        *(d->blocks + ncur) = block_get();
    }
    d->nblocks = nreq;
}

static void dynarr_tryshrink(struct dynarr *d) {
    
}

#define WRITE_TO_BLOCK(d, nblock, nbyte, data) \
    *(*((d)->blocks + nblock) + nbyte) = data;

static void dynarr_write(struct dynarr *d, char data, int index) {
    int nblock = index >> BLOCK_SHIFT;
    int nbyte = index % BLOCK_SIZE

    if (data == BLANK) {
        if (index < size)
            WRITE_TO_BLOCK(d, nblock, nbyte, data);
        if (index == size - 1) {
            while(...)
        }
    } else {
    }
    if (!IN_RANGE(d, index)) {
        dynarr_grow(d, index);
    }

    *(*(d-> blocks + nblock) + nbyte) = data;
}

struct tape *tape_create() {
    struct tape *t = malloc(sizeof(*t));
    
    dynarr_init(&(t->l));
    dynarr_init(&(t->r));

    return t;
}

void tape_destroy(struct tape *t) {
    dynarr_free(&(t->l));
    dynarr_free(&(t->r));
}

void tape_write(struct tape *t, char data, int index) {
}
