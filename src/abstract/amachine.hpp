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
        sat_sym_var = 0,
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
        cat_sym = 0,
        cat_call
    };
    CallArg(Sym sym): type(cat_sym), sym(sym) { }
    CallArg(std::unique_ptr<Call> p): type(cat_call), call(std::move(p)) { }
    CallArg() = default;
    CallArg(const CallArg &that);
    CallArg(CallArg &&that) = default;
    CallArg &operator=(const CallArg &that);
    CallArg &operator=(CallArg &&that) = default;
    Type type;
    Sym sym;
    std::unique_ptr<Call> call;
    void apply_chr(int arg_ind, unsigned char imm);
    void apply_state(int arg_ind, const Call &newcall);
};

class Call {
public:
    enum Type {
        ct_state_var = 0,
        ct_state_imm,
        ct_state_accept,
        ct_state_reject
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
    Primitive(Sym sym): type(pt_print), sym(sym) { }
    Primitive() = default;
    Primitive(const Primitive &that) = default;
    Primitive(Primitive &&that) = default;
    Primitive &operator=(const Primitive &that) = default;
    Primitive &operator=(Primitive &&that) = default;
    Type type;
    Sym sym;
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

class Sym {
public:
    Sym() = default;
    Sym(bool var, int value): var(var), value(value) { }
    Sym(const Sym &that) = default;
    Sym(Sym &&that) = default;
    Sym &operator=(const Sym &that) = default;
    Sym &operator=(const Sym &&that) = default;
    bool var;
    int value;
}

class SymSpec {
public:
    enum Type {
        cs_char = 0,
        cs_charrange,
        cs_charall
    };
    SymSpec(): type(cs_charall) { }
    SymSpec(Sym l): type(cs_char), left(l) { }
    SymSpec(Sym l, Sym r): type(cs_charrange), left(l), right(r) { }
    Type type;
    Sym left;
    Sym right;
}

class Branch {
public:
    Branch() = default;
    Branch(const Branch &that) = default;
    Branch(Branch &&that) = default;
    Branch &operator=(const Branch &that) = default;
    Branch &operator=(Branch &&that) = default;
    std::vector<SymSpec> syms;
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
