#ifndef UCHAR_VEC_H
#define UCHAR_VEC_H

#define T unsigned char
#define P(name) uchar_ ## name
#include "vec.h"
#undef T
#undef P

#endif
