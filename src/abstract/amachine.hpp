#ifndef AMACHINE_HPP
#define AMACHINE_HPP

#include <memory>
#include <string>
#include <vector>

class StateArg {
public:
    enum Type {
        sat_chr_var = 0,
        sat_state_var
    };
    StateArg(Type t, std::string n): type(t), name(n) { }
    const Type type;
    const std::string name;
};

class Call;

class CallArg {
public:
    enum Type {
        cat_chr_var = 0,
        cat_chr_imm,
        cat_call
    };
    Type type;
    int index;
    unsigned char imm;
    const std::unique_ptr<Call> call;
    void apply_chr(int arg_ind, unsigned char imm);
    void apply_state(int arg_ind, const string &name);
};

class Call {
public:
    enum Type {
        ct_state_var = 0,
        ct_state_imm
    };
    Type type;
    int index;
    std::string name;
    std::vector<CallArg> args;
    void apply_chr(int arg_ind, unsigned char imm);
    void apply_state(int arg_ind, const string &name);
};

class Primitive {
public:
    enum Type {
        pt_movel = 0,
        pt_mover,
        pt_print
    };
    Type type;
    CallArg arg;
    void apply_chr(int arg_ind, unsigned char imm);
};

class Action {
public:
    std::vector<Primitive> primitives;
    const std::unique_ptr<Call> call;
    void apply_chr(int arg_ind, unsigned char imm);
    void apply_state(int arg_ind, const string &name);
};

class Branch {
public:
    std::vector<CallArg> chars;
    Action action;
    void apply_chr(int arg_ind, unsigned char imm);
    void apply_state(int arg_ind, const string &name);
};

class State {
public:
    const std::string name;
    std::vector<StateArg> args;
    std::vector<Branch> branches;
    Action deflt;
};

#endif
