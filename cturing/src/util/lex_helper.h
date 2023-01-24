#ifndef LEX_HELPER_H
#define LEX_HELPER_H

#include "lex.h"

#define RANGE(c, l, h) ((c) >= (l) && (c) <= (h))
#define IS_IDENT1(c) (RANGE(c, 'A', 'Z') || RANGE(c, 'a', 'z') || (c) == '_')
#define IS_IDENT2(c) (IS_IDENT1(c) || RANGE(c, '0', '9'))
#define IS_HEX(c) (RANGE(c, '0', '9') || RANGE(c, 'a', 'f') || RANGE(c, 'A', 'F'))

void newtoken(Lexer *lx);
char peek(Lexer *lx);
void back(Lexer *lx);
void commit(Lexer *lx);
void commit_last(Lexer *lx);
void revert(Lexer *lx);
int test_keyword(Lexer *lx, const char *keyword);
int handle_sym(Lexer *lx);
int handle_ident(Lexer *lx);
void remove_junk(Lexer *lx);

#endif
