#ifndef CONCRETE_MACHINE_HPP
#define CONCRETE_MACHINE_HPP

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "concrete/tape.hpp"

enum PrimitiveType {
    movel = 0,
    mover = 1,
    print = 2
};

class Primitive {
public:
    Primitive(PrimitiveType t, unsigned char sym = '\0'): type(t), sym(sym) { }
    friend std::ostream &operator<<(std::ostream &os, const Primitive &p);
    PrimitiveType type;
    unsigned char sym;
};

enum TerminateType {
    cont = 0,
    accept = 1,
    reject = 2,
    fail = 3,
};

class State;
class Machine;

class Branch {
public:
    std::ostream &out(std::ostream &os, const Machine &m) const;
    bool def = false;
    std::set<unsigned char> syms;
    std::vector<Primitive> primitives;
    TerminateType term;
    std::vector<State>::size_type next;
};

class State {
public:
    std::ostream &out(std::ostream &os, const Machine &m) const;
    std::string name;
    std::vector<Branch> branches;
};

class Machine {
public:
    void add_state(State &s) { states.push_back(s); }
    State &add_state_inplace() { states.emplace_back(); return states.back(); }
    const State &get_state(std::vector<State>::size_type index) const { return states[index]; }
    std::vector<State>::size_type size() const { return states.size(); }
    friend std::ostream &operator<<(std::ostream &os, const Machine &m);
private:
    std::vector<State> states;
};

class Execution {
public:
    Execution(const Machine *m): m(m), tape() { }
    Execution(const Machine *m, const std::string &input): m(m), tape(input) { }
    void step();
    Tape::size_type pos() const { return _pos; }
    TerminateType term() const { return _term; }
    friend std::ostream &operator<<(std::ostream &os, const Execution &ex);
private:
    const Machine *m;
    Tape tape;
    Tape::size_type _pos = 0;
    TerminateType _term = TerminateType::cont;
    std::vector<State>::size_type state_idx = 0;
};

#endif
