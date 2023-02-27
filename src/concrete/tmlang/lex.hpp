#ifndef TMLANG_LEX_HPP
#define TMLANG_LEX_HPP

#include <iostream>
#include <string>
#include <memory>

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
    lbracket = 9,
    rbracket = 10,
    comma = 11,

    def = 12,
    accept = 13,
    reject = 14,
    
    range = 15,
};

struct position {
    size_t off = 0, lineno = 0, lineoff = 0;
    void clear() { off = lineno = lineoff = 0; }
};

class Lexer;

class Token {
    friend class Lexer;
public:
    Token (const Token &that)=default;
    Token &operator=(const Token &that)=default;

    static std::string name(TokenType t);
    friend std::ostream &operator<<(std::ostream &os, const Token &t);
    
    std::string substring() const;
    std::ostream &perror(std::ostream &os, const std::string &msg) const;
    TokenType type() const { return _type; }
    void decrease_range(size_t front, size_t back);
private:
    Token() = default;
    Token(std::shared_ptr<const std::string> s): _s(s) { clear(); }
    void clear() { _type = TokenType::error; _pos.clear(); _len = 0; }
    
    TokenType _type;
    struct position _pos;
    size_t _len;
    std::shared_ptr<const std::string> _s;
};

class Lexer {
public:
    Lexer(const std::string &s)
        { _s = std::make_shared<const std::string>(s); _tok(_s); lex(); }
    const Token &tok() const { return _tok; }
    TokenType lex();
private:
    char peek();
    void back();
    void commit() { _comm = _curr; _tok._len = _comm.off - _tok._pos.off; }
    void commit_last() { back(); commit(); }
    void revert() { _curr = _comm; }
    
    bool test_keyword(const std::string &keyword);
    TokenType handle_sym();
    TokenType handle_ident();
    TokenType handle_string_ident();
    void remove_junk();
    
    Token _tok;
    struct position _curr, _comm;
    std::shared_ptr<const std::string> _s;
};

#endif // LEX_HPP
