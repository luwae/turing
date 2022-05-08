#ifndef PARSE_HPP
#define PARSE_HPP

#include <string>
#include <vector>

namespace parse {

enum StateArgType {
    chr_var = 0;
    state_var = 1;
};

class StateArg {
public:
    StateArg(StateArgType t, const std::string &n): type(t), name(n) { }
    const StateArgType type;
    const std::string name;
};

enum CallArgType {
    chr_var = 0;
    chr_imm = 1;
    call = 2;
};

class Call;

class CallArg {
public:
    const CallArgType type;
    const union {
        int arg_index;
        unsigned char imm;
        Call call;
    } arg;
};

enum CallType {
    state_var = 0;
    state_imm = 1;
};

class Call {
public:
    const CallType type;
    const union {
        int arg_index;
        const std::string name;
    } toplevel;
    const std::vector<CallArg> args;
};

enum PrimitiveType {
    movel = 0,
    mover = 1,
    print = 2
};

class Primitive {
public:
    const PrimitiveType type;
    const CallArg arg;
};

class Action {
public:
    const std::vector<Primitives> primitives;
    const Call call;
};

class Branch {
public:
    const std::vector<CallArg> chars;
    const Action action;
};

class State {
public:
    State(const std::string &n): name(n) { }
private:
    const std::string name
    const std::vector<StateArg> args;
    const std::vector<Branch> branches;
    const Action deflt;
};

class Parser {
public:
    Parser(const std::string &input): lx(input) { parse(); }
private:
    void parse();
    void parseStateargs(State *state);
    void parseStatebody(State *state);
    Lexer lx;
    map<string, State *> states;
};

}

#endif // PARSE_HPP
