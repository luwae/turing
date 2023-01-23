#include "branch.h"

#define T Branch
#define P(name) branch_ ## name
#define VEC_IMPL
#include "vec.h"
#undef VEC_IMPL
#undef T
#undef P
