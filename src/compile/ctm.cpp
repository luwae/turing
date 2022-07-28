#include <iostream>

#include "machine.hpp"

#define IS_GOTO(b) (((b) & 0x80) == 0x80)
#define IS_PRINT(b) (((b) & 0xc0) == 0x40)
#define IS_PRINT_VAR(b) (((b) & 0xd0) == 0x50)
#define IS_MOVE(b) (((b) & 0xe0) == 0x20)
#define IS_CHAR(b) (((b) & 0xf0) == 0x10)

static const unsigned char * const HEADER = "CTM";

// TODO use named constants and macros instead of weird hex values

bool ctm_load_u8(unsigned int &num, istream &is) {
    unsigned char c;
    if (!is.get(c)) return false;
    num = c;
    return true;
}

bool ctm_load_u16(unsigned int &num, istream &is) {
    unsigned char c;
    if (!is.get(c)) return false;
    num = c;
    if (!is.get(c)) return false;
    num |= ((unsigned int) c) << 8;
    return true;
}

bool ctm_load_u24(unsigned int &num, istream &is) {
    unsigned char c;
    if (!is.get(c)) return false;
    num = c;
    if (!is.get(c)) return false;
    num |= ((unsigned int) c) << 8;
    if (!is.get(c)) return false;
    num |= ((unsigned int) c) << 16;
    return true;
}

bool ctm_load_u32(unsigned int &num, istream &is) {
    unsigned char c;
    if (!is.get(c)) return false;
    num = c;
    if (!is.get(c)) return false;
    num |= ((unsigned int) c) << 8;
    if (!is.get(c)) return false;
    num |= ((unsigned int) c) << 16;
    if (!is.get(c)) return false;
    num |= ((unsigned int) c) << 24;
    return true;
}

#define ENCODE_END 1
#define ENCODE_HEADER 2
#define ENCODE_EXPECTED_STATE 3

int ctm_decode_branchspec(State &s, istream &is) {
    // TODO
}

int ctm_decode_state(TuringMachine &tm, istream &is) {
    unsigned char c;
    int status;
    State s;

    while (true) { // tail call
        if (is.peek() == EOF && is.peek == 0x10) {
            tm.add_state(s);
            return 0;
        } else {
            status = ctm_decode_branchspec(s, is);
            if (status) return status;
            status = ctm_decode_branchbody(s, , is);
            if (status) return status;
        }
    }
}

int ctm_decode_machine(TuringMachine &tm, istream &is) {
    unsigned char c;
    int status;
    if (!is.get(c)) return ENCODE_END;
    if (c != 0x10) return ENCODE_EXPECTED_STATE;
    status = ctm_decode_state(tm, is);
    if (status) return status;
    return ctm_decode_machine2(tm, is);
}

int ctm_decode(TuringMachine &tm, istream &is) {
    unsigned char c;
    for (int i = 0; i < 3; i++) { // read magic number
        if (!is.get(c)) return ENCODE_END;
        if (c != HEADER[i]) return ENCODE_HEADER;
    }
    if (!is.get(c)) return ENCODE_END; // version byte (ignored for now)

    return ctm_decode_machine(tm, is);
}
