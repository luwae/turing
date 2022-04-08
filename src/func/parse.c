#include <lex.h>

struct lex lx;

struct state {
    // TODO
}

struct state *parse(char *input) {
    lex_init(&lx, input);
    // TODO
}

struct state parseState() {
    expect(T_IDENT);
    // TODO
}
