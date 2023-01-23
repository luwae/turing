#ifndef STATE_H
#define STATE_H

#include "uchar_vec.h"
#include "branch_vec.h"

typedef struct state State;
struct state {
    struct uchar_vec name;
    struct branch_vec branches;
};

void State_init(State *s);
void State_destroy(State *s);

#endif
