#ifndef AMACHINE_HPP
#define AMACHINE_HPP

#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <vector>

class StateArg {
public:
    enum Type {
        sat_chr_var = 0,
        sat_state_var
    };
    StateArg(Type t, std::string n): type(t), name(n) { }
    StateArg() = default;
    StateArg(const StateArg &that) = default;
    StateArg &operator=(const StateArg &that) = default;
    StateArg &operator=(StateArg &&that) = default;
    Type type;
    std::string name;
};

class Call;

class CallArg {
public:
    enum Type {
        cat_chr_var = 0,
        cat_chr_imm,
        cat_call
    };
    CallArg(int ind): type(cat_chr_var), index(ind) { }
    CallArg(unsigned char c): type(cat_chr_imm), imm(c) { }
    CallArg(std::unique_ptr<Call> p): type(cat_call), call(std::move(p)) { }
    CallArg() = default;
    CallArg(const CallArg &that);
    CallArg(CallArg &&that) = default;
    CallArg &operator=(const CallArg &that);
    CallArg &operator=(CallArg &&that) = default;
    Type type;
    int index;
    unsigned char imm;
    std::unique_ptr<Call> call;
    void apply_chr(int arg_ind, unsigned char imm);
    void apply_state(int arg_ind, const Call &newcall);
};

class Call {
public:
    enum Type {
        ct_state_var = 0,
        ct_state_imm
    };
    Call() = default;
    Call(const Call &that) = default;
    Call(Call &&that) = default;
    Call &operator=(const Call &that) = default;
    Call &operator=(Call &&that) = default;
    Type type;
    int index;
    std::string name;
    std::vector<CallArg> args;
    void apply_chr(int arg_ind, unsigned char imm);
    void apply_state(int arg_ind, const Call &newcall);
};

class Primitive {
public:
    enum Type {
        pt_movel = 0,
        pt_mover,
        pt_print
    };
    Primitive(Type t): type(t) { }
    Primitive(int ind): type(pt_print), arg(ind) { }
    Primitive(unsigned char c): type(pt_print), arg(c) { }
    Primitive() = default;
    Primitive(const Primitive &that) = default;
    Primitive(Primitive &&that) = default;
    Primitive &operator=(const Primitive &that) = default;
    Primitive &operator=(Primitive &&that) = default;
    Type type;
    CallArg arg;
    void apply_chr(int arg_ind, unsigned char imm);
};

class Action {
public:
    Action() = default;
    Action(const Action &that);
    Action(Action &&that) = default;
    Action &operator=(const Action &that);
    Action &operator=(Action &&that) = default;
    std::vector<Primitive> primitives;
    std::unique_ptr<Call> call;
    void apply_chr(int arg_ind, unsigned char imm);
    void apply_state(int arg_ind, const Call &newcall);
};

class Branch {
public:
    Branch() = default;
    Branch(const Branch &that) = default;
    Branch(Branch &&that) = default;
    Branch &operator=(const Branch &that) = default;
    Branch &operator=(Branch &&that) = default;
    std::vector<CallArg> chars;
    Action action;
    void apply_chr(int arg_ind, unsigned char imm);
    void apply_state(int arg_ind, const Call &newcall);
};

class Substitute {
public:
    Substitute() = default;
    Substitute(const Substitute &that);
    Substitute(Substitute &&that) = default;
    Substitute &operator=(const Substitute &that);
    Substitute &operator=(Substitute &&that) = default;
    Substitute(unsigned char i, std::unique_ptr<Call> c):
        imm(i), call(std::move(c)) {}
    unsigned char imm;
    std::unique_ptr<Call> call;
};

class State {
public:
    State() = default;
    State(const State &that) = default;
    State(State &&that) = default;
    State &operator=(const State &that) = default;
    State &operator=(State &&that) = default;
    std::string name;
    std::vector<StateArg> args;
    std::vector<Substitute> subs;
    std::vector<Branch> branches;
    Action deflt;
    void apply_chr(unsigned char imm);
    void apply_state(const Call &newcall);
    std::string rname() const;
    void expand(std::ostream &os, const std::vector<State> &states, std::set<std::string> &rstates);
};

std::ostream &operator<<(std::ostream &os, const StateArg &sa);
std::ostream &operator<<(std::ostream &os, const CallArg &ca);
std::ostream &operator<<(std::ostream &os, const Call &c);
std::ostream &operator<<(std::ostream &os, const Primitive &p);
std::ostream &operator<<(std::ostream &os, const Action &a);
std::ostream &operator<<(std::ostream &os, const Branch &b);
std::ostream &operator<<(std::ostream &os, const State &s);

#endif
