#include <stdio.h>
#include "lex.h"

#define RANGE(c, l, h) ((c) >= (l) && (c) <= (h))
#define IS_IDENT1(c) (RANGE(c, 'A', 'Z') || RANGE(c, 'a', 'z') || (c) == '_')
#define IS_IDENT2(c) (IS_IDENT1(c) || RANGE(c, '0', '9'))
#define IS_HEX(c) (RANGE(c, '0', '9') || RANGE(c, 'a', 'f') || RANGE(c, 'A', 'F'))

void Token_error(const Token *tok, const char *msg) {
    if (tok->type == T_EOF) {
        fprintf(stderr, "eof: %s\n", msg);
        return;
    }
    
    fprintf(stderr, "%lu:%lu: %s\n | ", tok->lineno, tok->off - tok->lineoff + 1, msg);
    for (size_t index = tok->lineoff; tok->s[index] != '\n' && tok->s[index] != '\0'; index++)
        fputc(tok->s[index], stderr);
    fprintf(stderr, "\n | ");
    for (size_t i = 0; i < tok->off - tok->lineoff; i++)
        fputc(' ', stderr);
    for (size_t i = 0; i < tok->len; i++)
        fputc('~', stderr);
    fputc('\n', stderr);
}

static void newtoken(Lexer *lx) {
    lx->tok.type = T_ERROR;
    lx->tok.s = lx->s;
    lx->tok.off = lx->off;
    lx->tok.len = 0;
    lx->tok.lineno = lx->lineno;
    lx->tok.lineoff = lx->lineoff;
}

static char getch(Lexer *lx) {
    ++lx->tok.len;
    return lx->s[lx->off++];
}

static void ungetch(Lexer *lx) {
    --lx->tok.len;
    --lx->off;
}

static int test_keyword(Lexer *lx, const char *keyword) {
    const char *s = lx->s + lx->off;
    while (*keyword)
        if (*s++ != *keyword++)
            return 0;
    if (IS_IDENT2(*s)) // guard against identifiers
        return 0;
    lx->tok.len = s - &lx->s[lx->off];
    lx->off += lx->tok.len;
    return 1;
}

// function left with '\0' or printable char except ' ' and '#'
static void remove_junk(Lexer *lx) {
    const char *const s = lx->s;
    while ((s[lx->off] != '\0' && s[lx->off] <= ' ') || s[lx->off] == '#') {
        if (s[lx->off] == '#') {
            ++lx->off;
            while (s[lx->off] != '\0' && s[lx->off] != '\n')
                lx->off++;
        } else if (s[lx->off] == '\n') {
            lx->lineno++;
            lx->lineoff = ++lx->off;
        } else {
            ++lx->off;
        }
    }
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

static int handle_sym(Lexer *lx) {
    char c1 = getch(lx);
    if (c1 == 'x') {
        char c2 = getch(lx);
        if (IS_HEX(c2)) {
            char c3 = getch(lx);
            if (IS_HEX(c3)) {
                char c4 = getch(lx);
                if (c4 == '\'') {
                    lx->tok.type = T_SYM;
                    return T_SYM;
                }
            }
        }
    } else if (RANGE(c1, ' ', '~')) {
        char c2 = getch(lx);
        if (c2 == '\'') {
            lx->tok.type = T_SYM;
            return T_SYM;
        }
    }
    ungetch(lx);
    return T_ERROR;
}

static int handle_string_ident(Lexer *lx) {
    char c;
    do {
        c = getch(lx);
        if (c == '\"') {
            lx->tok.type = T_IDENT;
            return T_IDENT;
        }
    } while (RANGE(c, ' ', '~'));
    ungetch(lx);
    return T_ERROR;
}

static int handle_ident(Lexer *lx) {
    char c;
    do {
        c = getch(lx);
    } while (IS_IDENT2(c));
    ungetch(lx);
    lx->tok.type = T_IDENT;
    return T_IDENT;
}

void Lexer_init(Lexer *lx, const char *s) {
    lx->off = lx->len = lx->lineno = lx->lineoff = 0;
    lx->s = s;
    lx->done = 0;
    newtoken(lx);
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
