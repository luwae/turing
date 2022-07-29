#ifndef PARSE_HPP
#define PARSE_HPP

#include <string>
#include <memory>
#include <vector>

#include "lex.hpp"
#include "amachine.hpp"

namespace parse {

class Parser {
public:
    Parser(const std::string &input): lx(input) { parse(); }
    std::vector<State> states;
private:
    void parse();
    void parseStateargs(State &s);
    void parseStatebody(State &s);
    CallArg parseChar(State &s);
    void parsePrimitives(State &s, Action &a);
    void parseCharargs(State &s, Branch &b);
    std::unique_ptr<Call> parseCall(State &s);
    lex::Lexer lx;
};

}

#endif // PARSE_HPP
