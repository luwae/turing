#ifndef BRANCH_VEC_H
#define BRANCH_VEC_H

#include "branch.h"

#define T Branch
#define P(name) branch_ ## name
#include "vec.h"
#undef T
#undef P

#endif
