#ifndef BRANCH_H
#define BRANCH_H

#include "uchar_vec.h"
#include "prim_vec.h"

#define TERM_CONT 0
#define TERM_ACC 1
#define TERM_REJ 2
#define TERM_FAIL 3

typedef struct branch Branch;
struct branch {
    int symset_invert;
    struct uchar_vec syms;
    struct prim_vec primitives;
    int term;
    size_t next;
};

void Branch_init(Branch *b, int symset_invert, int term, size_t next);
void Branch_destroy(Branch *b);

#endif
