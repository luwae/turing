#include "machine.h"

void Machine_init(Machine *m) {
    state_vec_init(&m->states);
}

void Machine_destroy(Machine *m) {
    state_vec_destroy(&m->states, State_destroy);
}
