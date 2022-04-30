#ifndef TURING_LEX_HPP
#define TURING_LEX_HPP

#include <iostream>
#include <fstream>
#include <string>

namespace lex {

enum TokenType: char {
    eof = 0,
    error = 1,
    ident = 2,
    chr_imm = 3,
    chr_var = 4,
    print = '=',
    mover = '>',
    movel = '<',
    lcurly = '{',
    rcurly = '}',
    lpar = '(',
    rpar = ')',
    lbra = '[',
    rbra = ']',
    comma = ','
};

class Token {
    friend class Lexer;
    friend std::ostream &operator<<(std::ostream &, const Token &);
public:
    using size_type = std::string::size_type;
    Token(): type(eof), len(0), begin(nullptr) { }
    Token(TokenType t, size_type l, const char *b):
        type(t), len(l), begin(b) { }
    void toString(std::string &s) const
        { s = std::string(begin, len); }
private:
    TokenType type;
    size_type len;
    const char *begin;
};

class Lexer {
public:
    Lexer(const char *in): input(in), cur(in) { lex(); };
    void lex();
    void expect(TokenType);
    void reset() { cur = input; lex(); }
    Token tok;
private:
    const char * const input;
    const char *cur;
};

} // namespace lex

#endif // TURING_LEX_HPP
