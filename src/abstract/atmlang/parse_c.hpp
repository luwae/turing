#ifndef PARSE_HPP
#define PARSE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "lex.hpp"
#include "../amachine.hpp"

namespace parse {

struct NameResolve {
    NameResolve(unsigned int si, unsigned int bi, lex::Token def): si(si), bi(bi), def(def) { }
    unsigned int si;
    unsigned int bi;
    lex::Token def;
};

struct StateDesc {
    unsigned int index;
    lex::Token def;
};

class Parser {
public:
    Parser(const std::string &input, TuringMachine &t): lx(input), tm(t) { parse(); }
private:
    void parse();
    lex::Lexer lx;
    std::map<std::string, StateDesc> statedescs;
    std::vector<NameResolve> resolve;
    std::vector<State> states;
    TuringMachine &tm;
};

}

#endif // PARSE_HPP
