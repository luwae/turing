#include "primitive.h"

#define T Primitive
#define P(name) prim_ ## name
#define VEC_IMPL
#include "vec.h"
#undef VEC_IMPL
#undef T
#undef P
