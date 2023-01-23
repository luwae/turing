#include "execution.h"

/*
main:
  ['_'] < add
  [def] >

add:
  ['1'] ='0' <
  [def] ='1' accept
*/

int main() {
    Machine m;
    State s;
    Branch b;
    Machine_init(&m);
    State_init(&s);
    Branch_init(&b, 0, TERM_CONT, 1);
    *uchar_vec_pushresv(&b.syms) = '_';
    Primitive_movel(prim_vec_pushresv(&b.primitives));
    branch_vec_push(&s.branches, &b);
    Branch_init(&b, 1, TERM_CONT, 0);
    Primitive_mover(prim_vec_pushresv(&b.primitives));
    branch_vec_push(&s.branches, &b);
    *uchar_vec_pushresv(&s.name) = 'm';
    *uchar_vec_pushresv(&s.name) = 'a';
    *uchar_vec_pushresv(&s.name) = 'i';
    *uchar_vec_pushresv(&s.name) = 'n';
    state_vec_push(&m.states, &s);
    State_init(&s);
    Branch_init(&b, 0, TERM_CONT, 1);
    *uchar_vec_pushresv(&b.syms) = '1';
    Primitive_print(prim_vec_pushresv(&b.primitives), '0');
    Primitive_movel(prim_vec_pushresv(&b.primitives));
    branch_vec_push(&s.branches, &b);
    Branch_init(&b, 1, TERM_ACC, 0);
    Primitive_print(prim_vec_pushresv(&b.primitives), '1');
    branch_vec_push(&s.branches, &b);
    *uchar_vec_pushresv(&s.name) = 'a';
    *uchar_vec_pushresv(&s.name) = 'd';
    *uchar_vec_pushresv(&s.name) = 'd';
    state_vec_push(&m.states, &s);

    Execution e;
    Execution_init(&e, &m, "01011");

    while (e.term == TERM_CONT) {
        Execution_step(&e);
    }

    Execution_destroy(&e);
    Machine_destroy(&m);
}
