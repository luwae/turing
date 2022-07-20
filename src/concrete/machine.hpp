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
    Primitive(PrimitiveType t, unsigned char c): type(t), chr(c) { }
    PrimitiveType type;
    unsigned char chr;
};

class Action {
public:
    std::vector<Primitive> primitives;
    std::string next;
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
    Action deflt;
};

class TuringMachine {
public:
    using size_type = std::vector<State>::size_type;
    friend std::ostream &operator<<(std::ostream &os, const TuringMachine &tm);
    void add_state(State &s) { statemap.insert({s.name, states.size()}); states.push_back(s); }
    bool contains_state(const std::string &name) const
        { return statemap.find(name) != statemap.end(); }
    const State &get_state(size_type index) const { return states[index]; }
    int find_state(const std::string &name) const {
        auto f = statemap.find(name);
        if (f == statemap.end())
            return -1;
        return (int) f->second;
    }
private:
    std::vector<State> states;
    std::map<std::string, size_type> statemap;
};

class MachineExecution {
public:
    MachineExecution(const TuringMachine *tm): tm(tm), tape() { }
    MachineExecution(const TuringMachine *tm, const std::string &input): tm(tm), tape(input) { }
    MachineExecution(const TuringMachine *tm, const std::vector<unsigned char> &v): tm(tm), tape(v) { }
    void step();
    void step_to(const std::string &name);
    void reset() { pos = 0; done = false; state = 0; }
    Tape::size_type getPos() const { return pos; }
    bool isDone() const { return done; }
    friend std::ostream &operator<<(std::ostream &os, const MachineExecution &me);
private:
    const TuringMachine *tm;
    Tape tape;
    Tape::size_type pos = 0;
    bool done = false;
    TuringMachine::size_type state = 0;
};

std::ostream &operator<<(std::ostream &os, const Primitive &p);
std::ostream &operator<<(std::ostream &os, const Action &a);
std::ostream &operator<<(std::ostream &os, const Branch &b);
std::ostream &operator<<(std::ostream &os, const State &s);

#endif
