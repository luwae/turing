#ifndef MACHINE_HPP
#define MACHINE_HPP

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "tape.hpp"

enum PrimitiveType {
    pt_movel = 0,
    pt_mover = 1,
    pt_print = 2
};

class Primitive {
public:
    Primitive(PrimitiveType t, unsigned char c = '\0'): type(t), chr(c) { }
    PrimitiveType type;
    unsigned char chr;
};

enum TerminateType {
    term_cont,
    term_acc,
    term_rej,
};

class Action {
public:
    std::vector<Primitive> primitives;
    TerminateType term;
    unsigned int next;
};

class Branch {
public:
    std::set<unsigned char> chars;
    Action action;
};

class State {
public:
    std::string name;
    std::vector<Branch> branches;
    bool has_default = false;
    Action deflt;
};

class TuringMachine {
public:
    friend std::ostream &operator<<(std::ostream &os, const TuringMachine &tm);
    void add_state(State &s) { states.push_back(s); }
    const State &get_state(unsigned int index) const { return states[index]; }
    unsigned int size() const { return states.size(); }
private:
    std::vector<State> states;
};

class MachineExecution {
public:
    MachineExecution(const TuringMachine *tm): tm(tm), tape() { }
    MachineExecution(const TuringMachine *tm, const std::string &input): tm(tm), tape(input) { }
    MachineExecution(const TuringMachine *tm, const std::vector<unsigned char> &v): tm(tm), tape(v) { }
    void step();
    void reset() { pos = 0; term = TerminateType::term_cont; state = 0; }
    Tape::size_type get_pos() const { return pos; }
    TerminateType get_term() const { return term; }
    friend std::ostream &operator<<(std::ostream &os, const MachineExecution &me);
private:
    const TuringMachine *tm;
    Tape tape;
    Tape::size_type pos = 0;
    TerminateType term = TerminateType::term_cont;
    unsigned int state = 0;
};

std::ostream &operator<<(std::ostream &os, const Primitive &p);
std::ostream &operator<<(std::ostream &os, const Action &a);
std::ostream &operator<<(std::ostream &os, const Branch &b);
std::ostream &operator<<(std::ostream &os, const State &s);

#endif
