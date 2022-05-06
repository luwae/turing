#ifndef PARSE_HPP
#define PARSE_HPP

#include <string>
#include <vector>

namespace parse {

enum StateArgType {
    chr_var = 0;
    state = 1;
};

class StateArg {
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
    const CallArgType type;
    const union {
        std::string name;
        Call *call;
    } arg;
};

class Call {
    const std::string name;
    const std::vector<CallArg> args;
};

enum PrimitiveType {
    movel = 0,
    mover = 1,
    print = 2
};

class Primitive {
    const PrimitiveType type;
    const CallArg arg;
};

class Action {
    const std::vector<Primitives> primitives;
    const Call call;
};

class Branch {
    const std::vector<CallArg> chars;
    const Action action;
};

class State {
    const std::string name
    const std::vector<StateArg> args;
    const std::vector<Branch> branches;
    const Action deflt;
};

}

#endif // PARSE_HPP
