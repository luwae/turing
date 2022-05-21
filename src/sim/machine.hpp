#ifndef MACHINE_HPP
#define MACHINE_HPP

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
    const PrimitiveType type;
    const unsigned char chr;
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
    std::vector<State> states;
    std::map<std::string, std::vector<State>::size_type> statemap;
    int pos = 0;
    bool done = false;
    std::vector<State>::size_type state = 0;

    void step(Tape &t);
    void reset() { pos = 0; done = false; state = 0; }
    void print(const Tape &t) const;
};

#endif
