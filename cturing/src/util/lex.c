#include <stdio.h>
#include "lex_helper.h"

void Token_error(const Token *tok, const char *msg) {
    if (tok->type == T_EOF) {
        fprintf(stderr, "eof: %s\n", msg);
        return;
    }
    
    fprintf(stderr, "%lu:%lu: %s\n | ", tok->pos.lineno, tok->pos.off - tok->pos.lineoff + 1, msg);
    for (size_t index = tok->pos.lineoff; tok->s[index] != '\n' && tok->s[index] != '\0'; index++)
        fputc(tok->s[index], stderr);
    fprintf(stderr, "\n | ");
    for (size_t i = 0; i < tok->pos.off - tok->pos.lineoff; i++)
        fputc(' ', stderr);
    for (size_t i = 0; i < tok->len; i++)
        fputc('~', stderr);
    fputc('\n', stderr);
}

void Lexer_init(Lexer *lx, const char *s) {	
    lx->comm = lx->curr = (struct position) {.off = 0, .lineno = 1, .lineoff = 0};
    lx->neof = 0;
    lx->s = s;
    lx->done = 0;
    newtoken(lx);
}
