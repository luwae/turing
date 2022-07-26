#include <string>
#include <vector>

#include "tape.hpp"

enum TState {
    RUN,
    ACCEPT,
    REJECT
};

class MachineRuntime {
public:
    MachineRuntime(const unsigned char *data, size_t sz, const std::string &input):
        data(data), sz(sz) tape(input) { collect_offsets(); }
    void step();
private:
    const unsigned char * const data;
    size_t sz;
    const std::vector<size_t> offsets;
    Tape tape;
    // current state
    TState term;
    int state = 0;
    int pos = 0;

    void collect_offsets();
};

// TODO don't do this explicitly; just load it into a machine?
// TODO no, because we want to retain the efficient macro-operations
// TODO alternative: better TuringMachine class
void MachineRuntime::step() {
    if (term != TState::RUN)
        return;

    unsigned char chr = tape[pos];
    size_t idx = offsets[state] + 1;
    size_t action = 0;
    while (true) {
        if ((data[idx] & 0xf0) == 0x10) { // char opcode
            if ((data[idx] & 0x0f) == 0x00) { // state
                break; // this should not happen
            } else if ((data[idx] & 0x0f) < 0x0c) { // no w
                for (size_t i = idx + 1; i < idx + 1 + (data[idx] & 0x0f); i++) {
                    if (data[i] == chr) {
                        action = idx + 1 + (data[idx] & 0x0f);
                        break;
                    }
                }
                idx = idx + 1 + (data[idx] & 0x0f);
            } else if ((data[idx] & 0x0f) == 0x0c) { // reserved
                break; // this should not happen
            } else if ((data[idx] & 0x0f) == 0x0d) { // charrange
                if (chr >= data[idx + 2] && chr < data[idx + 2] + data[idx + 1]) {
                    action = idx + 3;
                    break;
                }
                idx += 3;
            } else if ((data[idx] & 0x0f) == 0x0e) { // chardeflt
                action = idx + 1;
                break;
            } else { // charw1
                for (size_t i = idx + 2; i < idx + 2 + data[idx + 1]; i++) {
                    if (data[i] == chr) {
                        action = idx + 2 + data[idx + 1];
                        break;
                    }
                }
                idx = idx + 2 + data[idx + 1];
            }
        }
    }

    if (action) {
        // skip further char specs
        while ((data[action] & 0xf0) == 0x10) {
            if ((data[action] & 0x0f) == 0x00) { // state
                break; // TODO error
            } else if ((data[action] & 0x0f) < 0x0c) { // no w
                action = action + 1 + (data[action] & 0x0f);
            } else if ((data[action] & 0x0f) == 0x0c) { // reserved
                break; // TODO error
            } else if ((data[action] & 0x0f) == 0x0d) { // charrange
                action += 3;
            } else if ((data[action] & 0x0f) == 0x0e) { // chardeflt
                action += 1;
            } else { // charw1
                action = action + 2 + data[action + 1];
            }
        }
    } else {
        // TODO reject
    }

    while (/* action is print or move or goto */) {
        // TODO
    }
}

void MachineRuntime::collect_offsets() {
    // NOTE: ww options are not implemented
    for (size_t i = 0; i < sz; i++) {
       if (data[i] & 0x80) { // goto
           if ((data[i] & 0x7f) >= 0x78 && (data[i] & 0x7f) < 0x7c) { // w
               i += (data[i] & 0x03) + 1;
           }
       } else if ((data[i] & 0xc0) == 0x40) { // print
           if (data[i] & 0x10) {
               if ((data[i] & 0x0f) == 0x00) { // ww
                    // NOTE: ww options are not implemented
               } else if ((data[i] & 0x0f) < 0x0c) {
                   i += (data[i] & 0x0f);
               } else if ((data[i] & 0x0f) == 0x0c) { // w1
                   i += data[i+1] + 1;
               } else if ((data[i] & 0x0f) == 0x0d) { // w2
                   i += data[i+1] + (data[i+2] << 8) + 2;
               } else if ((data[i] & 0x0f) == 0x0e) { // w3
                   i += data[i+1] + (data[i+2] << 8) + (data[i+3] << 16) + 3;
               } else { // w4
                   i += data[i+1] + (data[i+2] << 8) + (data[i+3] << 16) + (data[i+4] << 24) + 4;
               }
           } else { // not w
               i += 1;
           }
       } else if ((data[i] & 0xe0) == 0x20) { // move
           if ((data[i] & 0x0f) >= 0x0c) {
               i += (data[i] & 0x03) + 1;
           }
       } else if ((data[i] & 0xf0) == 0x10) { // char
           if ((data[i] & 0x0f) == 0x00) { // state
                offsets.push_back(i);
           } else if ((data[i] & 0x0f) < 0x0c) {
               i += (data[i] & 0x0f);
           } else if ((data[i] & 0x0f) == 0x0c) { // reserved
               // nothing
           } else if ((data[i] & 0x0f) == 0x0d) { // charrange
               i += 2;
           } else if ((data[i] & 0x0f) == 0x0e) { // chardeflt
               // nothing
           } else { // w
               i += data[i+1] + 1;
           }
       }
    }
}
