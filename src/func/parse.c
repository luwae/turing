#include "lex.h"
#include "list.h"

struct lex lx;

struct signature {
    char *name;
    int nargs;
    struct list args;
};

struct app {
    struct state *st;
    struct list args;
}

struct branch {
    int imm;
    union {
        int narg;
        char sym;
    } u;
    struct app app;
};

struct state {
    struct signature sig;
    int high_order;
    union {
        struct app branch;
        struct list branches;
    } body;
}

struct state *parse(char *input) {
    lex_init(&lx, input);
    // TODO
}

struct state parseState() {
    expect(T_IDENT);
    // TODO
}
