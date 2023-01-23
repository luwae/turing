#ifndef TAPE_H
#define TAPE_H

#include "uchar_vec.h"

typedef struct tape Tape;
struct tape {
    struct uchar_vec left;
    struct uchar_vec right;
};

int Tape_init(Tape *t, const char *s);
void Tape_destroy(Tape *t);
size_t Tape_size(Tape *t);
size_t Tape_rsize(Tape *t);
size_t Tape_lsize(Tape *t);
unsigned char *Tape_at(Tape *t, int index);
unsigned char *Tape_atresv(Tape *t, int index);
void Tape_print(Tape *t);

#endif
