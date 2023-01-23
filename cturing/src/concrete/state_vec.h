#ifndef STATE_VEC_H
#define STATE_VEC_H

#include "state.h"

#define T State
#define P(name) state_ ## name
#include "vec.h"
#undef T
#undef P

#endif
