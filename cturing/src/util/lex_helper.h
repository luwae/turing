#ifndef LEX_HELPER_H
#define LEX_HELPER_H

#include "lex.h"

void newtoken(Lexer *lx);
char peek(Lexer *lx);
void back(Lexer *lx);
void commit(Lexer *lx);
void commit_last(Lexer *lx);
void revert(Lexer *lx);
int test_keyword(Lexer *lx, const char *keyword);
void remove_junk(Lexer *lx);

#endif
