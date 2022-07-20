#include <string>
#include <vector>

#include "tape.hpp"

class MachineRuntime {
public:
    MachineRuntime(const unsigned char *data, size_t sz, const std::string &input):
        data(data), sz(sz) tape(input) { collect_states(); }
private:
    const unsigned char * const data;
    size_t sz;
    const std::vector<size_t> offsets;
    Tape tape;

    void collect_offsets();
};

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
