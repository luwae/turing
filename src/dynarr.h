#ifndef DYNARR_H
#define DYNARR_H

#define BLANK '_'

/**
 * A resizable array for a turing machine.
 */
struct dynarr {
    int size; //current size (last used index is size - 1)
    int range; //current range (power of 2)
    char *data;
};

struct dynarr *dynarr_new();
void dynarr_free(struct dynarr *d);
void dynarr_setchar(struct dynarr *d, char c, int index);
char dynarr_getchar(struct dynarr *d, int index);

#endif
