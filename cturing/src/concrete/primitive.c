#include "primitive.h"

void Primitive_movel(Primitive *p) {
    p->type = PT_MOVEL;
}

void Primitive_mover(Primitive *p) {
    p->type = PT_MOVER;
}

void Primitive_print(Primitive *p, unsigned char sym) {
    p->type = PT_PRINT;
    p->sym = sym;
}
