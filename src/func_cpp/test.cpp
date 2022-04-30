#include <iostream>

#include "lex.hpp"

using std::cout; using std::endl;
using lex::Token; using lex::TokenType;
using lex::Lexer;

int main() {
    Lexer lx("test(hello)");
    lx.expect(TokenType::eof);
    lx.expect(TokenType::rbra);
    cout << lx.tok << endl;
}
