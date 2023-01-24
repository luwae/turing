#include "lex_helper.h"

void newtoken(Lexer *lx) {
    lx->tok.type = T_ERROR;
    lx->tok.s = lx->s;
    lx->tok.len = 0;
    lx->tok.pos = lx->comm;
}

char peek(Lexer *lx) {
    char c = lx->s[lx->curr.off];
    if (c == '\0') {
        ++lx->neof;
        return c;
    }
    if (c == '\n') {
        ++lx->curr.lineno;
        lx->curr.lineoff = lx->curr.off + 1;
    }
    ++lx->curr.off;
    return c;
}

static inline void search_last_lineoff(Lexer *lx) {
    for (size_t i = lx->curr.off - 1; i >= lx->comm.off; --i) {
        if (lx->s[i] == '\n') {
            lx->curr.lineoff = i + 1;
            break;
        }
    }
}

void back(Lexer *lx) {
    if (lx->neof) {
        --lx->neof;
        return;
    }
    // cannot move before commit
    if (lx->curr.off == lx->comm.off)
        return;

    char c = lx->s[--lx->curr.off];
    if (c == '\n') {
        --lx->curr.lineno;
        lx->curr.lineoff = lx->comm.lineoff; // conservative estimate
        if (lx->curr.off > lx->comm.off) { // avoid overflow lx->curr.off == 0
            search_last_lineoff(lx);
        }
    }
}

void commit(Lexer *lx) {
    lx->comm = lx->curr;
    lx->tok.len = lx->comm.off - lx->tok.pos.off;
}

void commit_last(Lexer *lx) {
    back(lx);
    commit(lx);
}

void revert(Lexer *lx) {
    lx->curr = lx->comm;
}

int test_keyword(Lexer *lx, const char *keyword) {
    while (*keyword)
        if (peek(lx) != *keyword++) {
            revert(lx);
            return 0;
        }
    char after = peek(lx);
    if (IS_IDENT2(after)) { // guard against identifiers
        revert(lx);
        return 0;
    }
    commit_last(lx);
    return 1;
}

int handle_sym(Lexer *lx) {
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
    commit_last(lx);
    return T_ERROR;
}

int handle_ident(Lexer *lx) {
    char c;
    do {
        c = peek(lx);
    } while (IS_IDENT2(c));
    commit_last(lx);
    lx->tok.type = T_IDENT;
    return T_IDENT;
}

static inline void remove_comment(Lexer *lx) {
    char c;
    do {
        c = peek(lx);
        if (c == '\0') {
            back(lx);
            return;
        }
    } while (c != '\n');
}

// function left with '\0' or printable char except ' ' and '#'
// technically creates a "whitespace token", but we have no use for it
void remove_junk(Lexer *lx) {
    char c;
    do {
        c = peek(lx);
        if (c == '#') {
            remove_comment(lx);
        } else if (c == '\0' || RANGE(c, '!', '~')) {
            commit_last(lx);
            return;
        }
    } while (1);
}
