#ifndef TAPE_H
#define TAPE_H

#include <stdint.h>

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

struct tape *tape_create_empty();
void tape_destroy(struct tape *t);
void tape_write_string(struct tape *t, char *s, uint8_t *sym2num);
void tape_write(struct tape *t, uint8_t data, int index);
void tape_sprint(struct tape *t, char *buf, char *num2sym);


#endif
