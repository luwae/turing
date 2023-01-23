#include "execution.h"

int Execution_init(Execution *e, Machine *m, const char *s) {
    e->m = m;
    e->pos = 0;
    e->term = TERM_CONT;
    e->state = 0;
    return Tape_init(&e->tape, s);
}

void Execution_destroy(Execution *e) {
    Tape_destroy(&e->tape);
}

static int uchar_vec_find(struct uchar_vec *v, unsigned char c) {
    unsigned char *ptr;
    for (size_t i = 0; (ptr = uchar_vec_at(v, i)); i++)
        if (*ptr == c)
            return 1;
    return 0;
}

void Execution_step(Execution *e) {
    if (e->term != TERM_CONT)
        return;

    unsigned char sym = *Tape_at(&e->tape, e->pos);
    State *curr = state_vec_at(&e->m->states, e->state);
    Branch *b = NULL;
    unsigned char *c = NULL;
    for (size_t i = 0; (b = branch_vec_at(&curr->branches, i)); i++) {
        if (b->symset_invert && !uchar_vec_find(&b->syms, sym)) {
            break;
        }
        if (!b->symset_invert && uchar_vec_find(&b->syms, sym)) {
            break;
        }
    }

    if (!b) {
        e->term = TERM_FAIL;
        return;
    }

    Primitive *p;
    for (size_t i = 0; (p = prim_vec_at(&b->primitives, i)); i++) {
        switch (p->type) {
        case PT_MOVEL:
            e->pos--;
            break;
        case PT_MOVER:
            e->pos++;
            break;
        case PT_PRINT:
            *Tape_atresv(&e->tape, e->pos) = p->sym;
            break;
        }
    }
    Tape_atresv(&e->tape, e->pos); // update tape to fit
    
    e->term = b->term;
    if (e->term == TERM_CONT) {
        e->state = b->next;
    }
}
