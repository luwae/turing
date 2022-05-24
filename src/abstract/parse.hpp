#ifndef PARSE_HPP
#define PARSE_HPP

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "lex.hpp"

namespace parse {

enum StateArgType {
    sat_chr_var = 0,
    sat_state_var = 1
};

class StateArg {
public:
    StateArg(StateArgType t, std::string n): type(t), name(n) { }
    StateArgType type;
    std::string name;
};

enum CallArgType {
    cat_chr_var = 0,
    cat_chr_imm = 1,
    cat_call = 2
};

class Call;

class CallArg {
public:
    CallArg() = default;
    CallArg(unsigned char i): type(CallArgType::cat_chr_imm), imm(i) { }
    CallArg(int ind): type(CallArgType::cat_chr_var), index(ind) { }
    CallArg(std::unique_ptr<Call> c): type(CallArgType::cat_call), call(std::move(c)) { }
    CallArgType type;
    int index;
    unsigned char imm;
    std::unique_ptr<Call> call;
};

enum CallType {
    ct_state_var = 0,
    ct_state_imm = 1
};

class Call {
public:
    CallType type;
    int index;
    std::string name;
    std::vector<CallArg> args;
    std::unique_ptr<Call> duplicate() {
        Call *c = std::make_unique<Call>();
        c->type = this->type;
        c->name = this->name;
        for (const auto &ca : this->args) {
            c->args.push_pack(ca);
        }
    }
};

enum PrimitiveType {
    pt_movel = 0,
    pt_mover = 1,
    pt_print = 2
};

class Primitive {
public:
    Primitive() = default;
    Primitive(PrimitiveType t): type(t) { }
    Primitive(unsigned char i): type(PrimitiveType::pt_print), arg(i) { }
    Primitive(int ind): type(PrimitiveType::pt_print), arg(ind) { }
    PrimitiveType type;
    CallArg arg;
};

class Action {
public:
    std::vector<Primitive> primitives;
    std::unique_ptr<Call> call;
};

class Branch {
public:
    std::vector<CallArg> chars;
    Action action;
};

class State {
public:
    std::string name;
    std::vector<StateArg> args;
    std::vector<Branch> branches;
    Action deflt;
};

std::ostream &operator<<(std::ostream &os, const StateArg &sa);
std::ostream &operator<<(std::ostream &os, const Call &c);
std::ostream &operator<<(std::ostream &os, const CallArg &ca);
std::ostream &operator<<(std::ostream &os, const Primitive &p);
std::ostream &operator<<(std::ostream &os, const Action &a);
std::ostream &operator<<(std::ostream &os, const Branch &b);
std::ostream &operator<<(std::ostream &os, const State &s);

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
    Call *parseCall(State &s);
    lex::Lexer lx;
};

}

#endif // PARSE_HPP
