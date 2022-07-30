#ifndef PARSE_HPP
#define PARSE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "lex.hpp"
#include "machine.hpp"

namespace parse {

struct NameResolve {
    NameResolve(unsigned int *nextp, std::string &&name, lex::Token def): nextp(nextp), name(name), def(def) { }
    unsigned int *nextp;
    std::string name;
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
    void parse_statebody(State &s);
    void parse_actions(Action &a);
    void parse_nextstate(Action &a);
    lex::Lexer lx;
    std::map<std::string, StateDesc> states;
    std::vector<NameResolve> resolve;
    TuringMachine &tm;
};

}

#endif // PARSE_HPP
