
#include <stdexcept>

#include "lex.hpp"

namespace lex {

void Lexer::lex() {
    char c;
    
}

void Lexer::expect(TokenType type) {
    if (tok.type != type) {
        throw std::invalid_argument("wrong token");
    }
    lex();
}

std::ostream &operator<<(std::ostream &os,
        const Token &tok) {
    if (tok.type == eof) {
        os << "Token(eof)";
    } else if (tok.type == error) {
        os << "Token(error)";
    } else {
        std::string s;
        tok.toString(s);
        os << "Token(" << (int) tok.type << ", " << s << ")";
    }
    return os;
}

} // namespace lex
