#ifndef PARSE_HPP
#define PARSE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>

#include "lex.hpp"
#include "../amachine.hpp"

namespace parse {

struct StateDesc {
    unsigned int index;
    lex::Token def;
};

class Parser {
public:
    Parser(const std::string &input): lx(input) { parse(); }
private:
    void parse();
    void parse_s();
    void parse_stateargs(State &s);
    void parse_stateargs2(State &s);
    void parse_statebody(State &s);
    void parse_statebody2(State &s);
    int find_state_arg(State &s, StateArg::Type t, bool error = true);
    Sym parse_sym(State &s);
    void parse_symrange(State &s, Branch &b, Sym prev);
    void parse_symargs(State &s, Branch &b);
    void parse_actions(State &s, Branch &b);
    void parse_nextstate(State &s, Branch &b);
    std::unique_ptr<Call> parse_call(State &s);
    std::unique_ptr<Call> parse_callargs(State &s, std::unique_ptr<Call> c);
    std::unique_ptr<Call> parse_callargs2(State &s, std::unique_ptr<Call> c);
    std::unique_ptr<Call> parse_callargs3(State &s, std::unique_ptr<Call> c);
    lex::Lexer lx;
    std::map<std::string, StateDesc> statedescs;
    std::vector<State> states;
};

}

#endif // PARSE_HPP
