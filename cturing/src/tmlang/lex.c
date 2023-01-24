#include <stdio.h>
#include "util/lex_helper.h"
#include "tokentypes.h"

static struct { char c; int type; } single_chars[] = {
    {'<', T_MOVEL}, {'>', T_MOVER},
    {'{', T_LCURLY}, {'}', T_RCURLY},
    {'[', T_LBRACKET}, {']', T_RBRACKET},
    {'=', T_PRINT}, {',', T_COMMA},
    {'-', T_RANGE}
};

static struct { char *key; int type; } keywords[] = {
    {"def", T_DEF}, {"accept", T_ACCEPT}, {"reject", T_REJECT}
};

static int handle_sym(Lexer *lx) {
    char c1 = peek(lx);
    if (c1 == 'x') {
        char c2 = peek(lx);
        if (IS_HEX(c2)) {
            char c3 = peek(lx);
            if (IS_HEX(c3)) {
                char c4 = peek(lx);
                if (c4 == '\'') {
                    commit(lx);
                    lx->tok.type = T_SYM;
                    return T_SYM;
                }
            }
        }
    } else if (RANGE(c1, ' ', '~')) {
        char c2 = peek(lx);
        if (c2 == '\'') {
            commit(lx);
            lx->tok.type = T_SYM;
            return T_SYM;
        }
    }
    revert(lx);
    return T_ERROR;
}

static int handle_string_ident(Lexer *lx) {
    char c;
    do {
        c = peek(lx);
        if (c == '\"') {
            commit(lx);
            lx->tok.type = T_IDENT;
            return T_IDENT;
        }
    } while (RANGE(c, ' ', '~'));
    revert(lx);
    return T_ERROR;
}

static int handle_ident(Lexer *lx) {
    char c;
    do {
        c = peek(lx);
    } while (IS_IDENT2(c));
    commit_last(lx);
    lx->tok.type = T_IDENT;
    return T_IDENT;
}

int lex(Lexer *lx) {
    if (lx->done)
        return T_EOF;

    remove_junk(lx);

    newtoken(lx);
    char c = getch(lx);
    
    if (c == '\0') {
        ungetch(lx);
        lx->done = 1;
        lx->tok.type = T_EOF;
        return T_EOF;
    }
    
    for (size_t i = 0; i < sizeof(single_chars)/sizeof(single_chars[0]); i++) {
        if (single_chars[i].c == c) {
            lx->tok.type = single_chars[i].type;
            return single_chars[i].type;
        }
    }
    if (c == '\'') {
        return handle_sym(lx);
    }
    if (c == '\"')
        return handle_string_ident(lx);
    
    if (IS_IDENT1(c)) {
        ungetch(lx);
        for (size_t i = 0; i < sizeof(keywords)/sizeof(keywords[0]); i++) {
            if (test_keyword(lx, keywords[i].key)) {
                lx->tok.type = keywords[i].type;
                return keywords[i].type;
            }
        }
        getch(lx);
        return handle_ident(lx);
    }
    
    ungetch(lx);
    return T_ERROR;
}
