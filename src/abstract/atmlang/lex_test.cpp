#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "lex.hpp"

using std::cin; using std::cout; using std::endl;
using std::ifstream; using std::stringstream;
using std::string;

using namespace lex;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "usage: lex_test <machine file>" << endl;
        return -1;
    }

    ifstream file(argv[1]);
    stringstream ss;
    ss << file.rdbuf();
    string input = ss.str();

    Lexer lx(input);
    while (lx.gettok().gettype() != TokenType::eof
            && lx.gettok().gettype() != TokenType::error) {
        cout << lx.gettok().repr() << endl;
        lx.lex();
    }
}
