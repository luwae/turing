#ifndef EXECUTION_H
#define EXECUTION_H

#include "machine.h"
#include "tape.h"

typedef struct execution Execution;
struct execution {
    Machine *m;
    Tape tape;
    int pos;
    int term;
    size_t state;
};

int Execution_init(Execution *e, Machine *m, const char *s);
void Execution_destroy(Execution *e);
void Execution_step(Execution *e);

#endif
