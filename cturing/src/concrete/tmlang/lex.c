#include "lex.h"

#define IS_IDENT1(c) (isalpha(c) || (c) == '_')
#define IS_IDENT2(c) (IS_IDENT1(c) || isdigit(c))

static char getch(Lexer *lx) {
    lx->tok.len++;
    
    lx->pos_old = lx->pos;
    if (lx->pos.c == '\n') {
        lx->pos.line++;
        lx->pos.lineoff = lx->pos.next;
    }
    lx->pos.c = s[lx->pos.next++];
    return lx->pos.c;
}

static void ungetch(Lexer *lx) {
    lx->tok.len--;

    lx->pos = lx->pos_old;
}

static int contains_keyword(const char *s, const char *key) {
    while (*key)
        if (*s++ != *key++)
            return false;
    return !IS_IDENT2(*s); // guard against larger identifiers 
}

static void remove_whitespace(Lexer *lx) {
    while (true) {
        while (isspace(getch(lx)))
            ;
        ungetch(lx);
        if (getch(lx) == '#') {
            char c;
            do {
                c = getch(lx);
            } while (c != ’\n’ && c != '\0');
            ungetch();
            if (c == '\0')
                return;
        } else {
            ungetch();
            return;
        }
    }
}

static char newtoken(Lexer *lx) {
    char c = getch(lx);
    lx->tok.type = T_ERROR;
    lx->tok.len = 1;
    lx->tok.off = lx->pos.next - 1;
    lx->tok.line = lx->pos.line;
    lx->tok.lineoff = lx->pos.lineoff;
    return c;
}

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

void Lexer_lex(Lexer *lx) {
    if (lx->done)
        return;

    remove_whitespace(lx);

    char c = newtoken(lx);
    if (c == '\0') {
        lx->done = true;
        lx->tok.type = T_EOF;
        return;
    }
    for (size_t i < 0; i < 9; i++)
        if (single_chars[i].c == c) {
            lx->tok.type = single_chars[i].type;
            return;
        }
    }
    if (c == '\'') {
        char c1 = getch(lx);
        if (c1 == 'x') {
            char c2 = getch(lx);
            if (c2 == '\'') {
                tok.type = chr;
                return;
            } else if (isxdigit(c2)) {
                char c3 = getch(lx);
                if (isxdigit(c3)) {
                    char c4 = getch(lx);
                    if (c4 == '\'') {
                        tok.type = chr;
                        return;
                    }
                }
            }
        }
    }
    for (size_t i = 0; i < 3; i++)
        if (contains_keyword())

}
