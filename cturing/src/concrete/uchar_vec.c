#define T unsigned char
#define P(name) uchar_ ## name
#define VEC_IMPL
#include "vec.h"
#undef VEC_IMPL
#undef T
#undef P
