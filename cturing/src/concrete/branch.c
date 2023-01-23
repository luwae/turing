#include "branch.h"

void Branch_init(Branch *b, int symset_invert, int term, size_t next) {
    b->symset_invert = symset_invert;
    uchar_vec_init(&b->syms);
    prim_vec_init(&b->primitives);
    b->term = term;
    b->next = next;
}

void Branch_destroy(Branch *b) {
    uchar_vec_destroy(&b->syms, NULL);
    prim_vec_destroy(&b->primitives, NULL);
}
