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
    NameResolve(unsigned int *nextp, const std::string &&name): nextp(nextp), name(name) { }
    unsigned int *nextp;
    const std::string name;
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
    std::map<std::string, unsigned int> states;
    std::vector<NameResolve> resolve;
    TuringMachine &tm;
};

}

#endif // PARSE_HPP
