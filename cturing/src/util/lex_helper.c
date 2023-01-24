#include "lex_helper.h"

void newtoken(Lexer *lx) {
    lx->tok.type = T_ERROR;
    lx->tok.s = lx->s;
    lx->tok.len = 0;
    lx->tok.pos = lx->comm;
}

char peek(Lexer *lx) {
    char c = lx->s[lx->curr.off];
    if (c == '\0')
        return c;
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
    lx->tok.len = lx->comm.off - lx->tok.off;
}

void commit_last(Lexer *lx) {
    back(lx);
    commit(lx);
}

void revert(Lexer *lx) {
    lx->curr = lx->comm;
}

static int test_keyword(Lexer *lx, const char *keyword) {
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

// function left with '\0' or printable char except ' ' and '#'
static void remove_junk(Lexer *lx) {
    const char *const s = lx->s;
    char c;
    do {
        c = peek(lx);
        if (c == '#') {
            while ((c = peek(lx)) != '\0' && c != '\n')
                ;
            back(lx);
        } else if (c == '\0' || c > ' ') {
            commit_last();
            return;
        }
    } while (1);
}
