#ifndef LEX_HPP
#define LEX_HPP

#include <string>

namespace lex {

enum TokenType {
    eof = 0,
    error = 1,
    sym = 2,
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
    varsym = 14,
    
    def = 15,
    accept = 16,
    reject = 17,
    
    range = 18,
};

class Lexer;

class Token {
    friend class Lexer;
public:
    using size_type = std::string::size_type;

    Token()=default;
    Token (const Token &that)=default;
    Token &operator=(const Token &that)=default;

    static std::string name(TokenType t);
    std::string substring() const;
    std::string repr() const;
    std::ostream &perror(std::ostream &os, const std::string &msg) const;
    TokenType gettype() const { return type; }
private:
    TokenType type;
    size_type len;
    size_type offset;
    int line;
    size_type lineoff;
    const Lexer *lx;

    // only allow creation of tokens by lexer
    Token(TokenType type, size_type len, size_type offset, int line, size_type lineoff, const Lexer *lx):
        type(type), len(len), offset(offset), line(line), lineoff(lineoff), lx(lx) { }
};

class Lexer {
    friend class Token;
public:
    using size_type = std::string::size_type;
    Lexer(const std::string &input): s(input)
        { lex(); }
    const Token &gettok() { return tok; }
    void lex();
    void reset();
private:
    char getch();
    void ungetch();
    char newtoken();
    void remove_whitespace();
    struct {
        char c = '\0';
        size_type next = 0;
        int line = 1;
        size_type lineoff = 0;
    } pos, pos_old;
    const std::string s;
    bool done = false;
    Token tok;
};

}

#endif // LEX_HPP
