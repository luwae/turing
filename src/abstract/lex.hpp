#ifndef LEX_HPP
#define LEX_HPP

#include <string>

namespace lex {

enum TokenType {
    eof = 0,
    error = 1,
    chr_imm = 2,
    ident = 3,

    movel = 4,
    mover = 5,
    print = 6,
    lcurly = 7,
    rcurly = 8,
    lpar = 9,
    rpar = 10,
    lbracket = 11,
    rbracket = 12,
    comma = 13,
    chr_var = 14
};

class Lexer {
public:
    using size_type = std::string::size_type;
    Lexer(const std::string &input): s(input)
        { lex(); }
    TokenType toktype() const { return type; }
    std::string substring() const;
    void lex();
    void expect(TokenType);
    static std::string tokname(TokenType t);
private:
    std::string s;
    size_type cur = 0;
    bool done = false;
    
    inline void settoken(TokenType t, size_type l, size_type o)
        { type = t; len = l; offset = o; }
    TokenType type;
    size_type len;
    size_type offset;
};

}

#endif // LEX_HPP
