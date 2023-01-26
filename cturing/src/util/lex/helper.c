#include "util/lex/helper.h"

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
