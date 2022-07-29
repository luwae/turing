#ifndef PARSE_HPP
#define PARSE_HPP

#include <iostream>
#include <string>
#include <vector>

#include "lex.hpp"
#include "machine.hpp"

namespace parse {

class Parser {
public:
    Parser(const std::string &input, TuringMachine &t): lx(input), tm(t) { parse(); }
private:
    void parse();
    void parseStatebody(State &s);
    void parsePrimitives(State &s, Action &a);
    void parseCharargs(State &s, Branch &b);
    lex::Lexer lx;
    TuringMachine &tm;
};

}

#endif // PARSE_HPP
