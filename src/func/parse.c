#include "lex.h"
#include "arr.h"

struct lex lx;

#define ARG_STATE 0
#define ARG_CHR_IMM 1
#define ARG_CHR_VAR 2

struct signature {
    char *name;
    int nargs;
    struct arr args;
};

#define ARG_CHR 0
#define ARG_STATE 1

struct arg {
    int type; // var state, var sym, sym, app
    union {
        int narg;
        char sym;
        struct app app; 
    }
}

struct app {
    int imm;
    union {
        int narg;
        struct state *st;
    }
    struct arg *args;
};

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
    struct arr branches;
    struct app *deflt;
}


void state_free(struct state *s) {
    
}


struct arr *parse(char *input) {
    lex_init(&lx, input);
    struct arr *states = a_new_custom(NULL, );

    while (1) {
        switch (lx.cur.type) {
            case T_EOF:
                return sl;
            case T_IDENT:
                struct state *s = statelist_grow(sl);
                s->sig.name = strndup(lx.cur.s, lx.cur.len);
                lex(&lx);
                parse_Stateargs(s);
                break;
            default:
                error();
        }
    }
}

struct state {
    struct signature sig;
};

struct signature {
    char *name;
    struct arr args; 
}

void parse_Stateargs(struct state *s) {
    switch (lx.cur.type) {
        case T_LPAR:
            lex(&lx);
            parse_Stateargs2(s);
            break;
        case T_LCURLY:
            break;
        default:
            error();
    }
}

void parse_Stateargs2(struct state *s) {
    switch (lx.cur.type) {
        case T_IDENT:
        case T_CHR_VAR:
            a_append(&(s->sig.args), strndup(lx.cur.s, lx.cur.len)); 
            parse_Stateargs3(s);
            break;
        default:
            error();
    }
}

void parse_Stateargs3(struct state *s) {
    switch (lx.cur.type) {
        case T_LPAR:
            lex(&lx);
            break;
        case T_COMMA:
            lex(&lx);
            parse_Stateargs2(s);
            break;
        default:
            error();
    }
}


