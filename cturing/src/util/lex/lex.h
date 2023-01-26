#ifndef LEX_H
#define LEX_H

#include <stddef.h>
#include "util/lex/structs.h"

/**
 * Print gcc-style error to stderr.
 * @param tok the token that caused the error
 * @param msg the error message
 */
void Token_error(const Token *tok, const char *msg);

/**
 * Initialize lexer.
 * @param lx the lexer
 * @param s the input string
 */
void Lexer_init(Lexer *lx, const char *s);

/**
 * Generate another token from lexer.
 * @param lx the lexer
 * @return token type, equal to lx->tok.type
 */
int lex(Lexer *lx);

#endif
