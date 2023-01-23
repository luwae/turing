#ifndef PRIM_VEC_H
#define PRIM_VEC_H

#include "primitive.h"

#define T Primitive
#define P(name) prim_ ## name
#include "vec.h"
#undef T
#undef P

#endif
