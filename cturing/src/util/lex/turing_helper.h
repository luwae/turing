#ifndef UTIL_LEX_TURING_HELPER_H
#define UTIL_LEX_TURING_HELPER_H

#include "util/lex/helper.h"
#include "util/lex/turing_tokentypes.h"

int test_keyword(Lexer *lx, const char *keyword);
int handle_sym(Lexer *lx);
int handle_ident(Lexer *lx);
void remove_junk(Lexer *lx);

#endif
