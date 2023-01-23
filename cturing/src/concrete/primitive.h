#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#define PT_MOVEL 0
#define PT_MOVER 1
#define PT_PRINT 2

typedef struct primitive Primitive;
struct primitive {
    char type;
    unsigned char sym;
};

void Primitive_movel(Primitive *p);
void Primitive_mover(Primitive *p);
void Primitive_print(Primitive *p, unsigned char sym);

#endif
