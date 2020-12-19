#ifndef TAPE_H
#define TAPE_H

#define DYNARR_SIZE_MIN 8
#define BLANK '_'

/* A resizable array containing characters.
 */
struct dynarr {
    unsigned int size; //current size (last used index is size - 1)
    unsigned int range; //current maximal size, always a power of two
    char *data;
};

/* A tape ranging from -infinity to infinity.
 */
struct tape {
    struct dynarr l; //left half, containing (-infinity..-1]
    struct dynarr r; //right half, containing [0..infinity)
};

struct tape *tape_create_empty();
void tape_destroy(struct tape *t);
void tape_write(struct tape *t, char data, int index);
void tape_sprint(struct tape *t, char *buf);


#endif
