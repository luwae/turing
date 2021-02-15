#ifndef DYNARR_H
#define DYNARR_H

#define BLANK '_'

#define BLOCK_SIZE 256
#define BLOCK_SHIFT 8

struct block {
    char data[BLOCK_SIZE];
};

struct dynarr {
    unsigned int size;
    unsigned int nblocks;
    struct block **blocks;
}

void dynarr_init(struct dynarr *d);
void dynarr_free(struct dynarr *d);
void dynarr_write(struct dynarr *d, char data, int index);
char dynarr_read(struct dynarr *d, int index);
int dynarr_sprint(struct dynarr *d, char *buf);

#endif
