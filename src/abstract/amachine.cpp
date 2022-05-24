#include "amachine.hpp"

using std::string;

void CallArg::apply_chr(int arg_ind, unsigned char imm) {
    if (type == cat_chr_var) {
        if (arg_ind == index) {
            type = cat_chr_imm
            this->imm = imm;
        }
    } else if (type == cat_call && call) {
        call->apply_chr(arg_ind, imm);
    }
}

void CallArg::apply_state(int arg_ind, const string &name) {
    if (type == cat_call && call)
        call->apply_state(arg_ind, name);
}

void Call::apply_chr(int arg_ind, unsigned char imm) {
    for (const auto &ca : args) {
        ca.apply_chr(s, arg_ind, imm);
    }
}

void Call::apply_state(int arg_ind, const string &name) {
    if (type == ct_state_var) {
        if (arg_ind == index) {
            type = ct_state_imm;
            this->name = name
        }
    }
    for (const auto &ca : args) {
        ca.apply_state(s, arg_ind, name);
    }
}

void Primitive::apply_chr(int arg_ind, unsigned char imm) {
    if (type == pt_print) {
        arg.apply_chr(arg_ind, imm);
    }
}

void Action::apply_chr(int arg_ind, unsigned char imm) {
    for (const auto &p : primitives)
        p.apply_chr(arg_ind, imm);
    if (call)
        call->apply_chr(arg_ind, imm);
}

void Action::apply_state(int arg_ind, const string &name) {
    if (call)
        call->apply_chr(arg_ind, name);
}

void Branch::apply_chr(int arg_ind, unsigned char imm) {
    for (const auto &ca : chars)
        ca.apply_chr(arg_ind, imm);
    action.apply_chr(arg_ind, imm);
}

void Branch::apply_state(int arg_ind, const string &name) {
    for (const auto &ca : chars)
        ca.apply_state(arg_ind, name);
    action.apply_state(arg-ind, name);
}
