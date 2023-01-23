#include "state.h"

void State_init(State *s) {
    uchar_vec_init(&s->name);
    branch_vec_init(&s->branches);
}

void State_destroy(State *s) {
    uchar_vec_destroy(&s->name, NULL);
    branch_vec_destroy(&s->branches, Branch_destroy);
}
