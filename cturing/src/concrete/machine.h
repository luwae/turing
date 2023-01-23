#ifndef MACHINE_H
#define MACHINE_H

#include "state_vec.h"

typedef struct machine Machine;
struct machine {
    struct state_vec states;
};

void Machine_init(Machine *m);
void Machine_destroy(Machine *m);

#endif
