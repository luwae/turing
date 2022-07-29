#include <iostream>
#include <fstream>
#include <sstream>
#include "lex.hpp"

using std::cout; using std::endl;
using std::ifstream; using std::stringstream;
using lex::Lexer; using lex::TokenType;

int main() {
    ifstream file("../../machines/simple.atm");
    stringstream ss;
    ss << file.rdbuf();
    
    Lexer lx(ss.str());
    cout << "Token(" << lx.tokname(lx.toktype()) << ", "
         << lx.substring() << ")" << endl;
    do {
        lx.lex();
        cout << "Token(" << lx.tokname(lx.toktype()) << ", "
             << lx.substring() << ")" << endl;
    } while (lx.toktype() != TokenType::eof && lx.toktype() != TokenType::error);
}
