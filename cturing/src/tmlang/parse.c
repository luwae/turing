#include "util/lex/lex.h"

#define BUFSIZE 1024 * 1024
char s[BUFSIZE];
size_t ptr;

struct resolve {
    size_t state_idx;
    size_t branch_idx;
    Token def;
};

struct statedesc {
    char *s;
    size_t len;
    size_t idx;
    Token def;
};


typedef struct Parser Parser;
struct Parser {
    Lexer lx;
    statedesc_vec statedescs;
    resolve_vec resolves;
    state_vec states;
    Machine *m;
};

void Parser_init(Parser *p, const char *s, Machine *m) {
    Lexer_init(&p->lx, s);
    statedesc_vec_init(&p->statedescs);
    resolve_vec_init(&p->resolves);
    state_vec_init(&p->states);
    p->tm = tm;
    Machine_init(m);
}

// destroy, new, free

// semantic exactly opposite to normal strcmp to make it maximally fun

void real_ident(Token *t) {
    if (t->s[t->pos.off] == "\"") {
        ++t->pos.off;
        --t->len;
    }
}

int streq(const char *s1, size_t len1, const char *s2, size_t len2) {
    if (len1 != len2)
        return 0;
    for (size_t i = 0; i < len1; i++)
        if (s1[i] != s2[i])
            return 0;
    return 1;
}

ssize_t statedescs_find(const statedesc_vec *sds, const char *s, size_t len) {
    for (size_t i = 0; i < statedesc_vec_size(sds); i++) {
        const struct statedesc *curr = sds.at(i);
        if (streq(s, len, curr->s, curr->len))
            return i;
    }
    return -1;
}

int expect(const Lexer *lx, int type) {
    if (lx->tok.type != type) {
        Token_error(&lx->tok, "expected %d\n", type);
        return 0;
    }
    return 1;
}

int expect_and(Lexer *lx, int type) {
    int ret = expect(lx, type);
    lex(lx);
    return ret;
}

int parse(Parser *p) {
    do {
        if (!expect(&p->lx, T_IDENT))
            return -1;
        real_ident(&p->lx.tok);
        ssize_t found = statedescs_find(&p->statedescs, p->lx.tok.s + p->lx.tok.pos.off, p->lx.tok.len);
    }
}
