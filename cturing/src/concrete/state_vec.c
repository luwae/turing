#include "state.h"

#define T State
#define P(name) state_ ## name
#define VEC_IMPL
#include "vec.h"
#undef VEC_IMPL
#undef T
#undef P
