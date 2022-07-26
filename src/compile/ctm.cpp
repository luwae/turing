#include <iostream>

#include "machine.hpp"

#define IS_GOTO(b) (((b) & 0x80) == 0x80)
#define IS_PRINT(b) (((b) & 0xc0) == 0x40)
#define IS_MOVE(b) (((b) & 0xe0) == 0x20)
#define IS_CHAR(b) (((b) & 0xf0) == 0x10)

enum LoadError {
    le_header,
    le_unexpected_end,
};

static const unsigned char * const HEADER = "CTM";

// TODO use named constants and macros instead of weird hex values

bool ctm_load_u8(unsigned int &i, istream &is) {
    unsigned char c;
    if (!is.get(c)) return false;
    i = c;
    return true;
}

bool ctm_load_u16(unsigned int &i, istream &is) {
    unsigned char c;
    if (!is.get(c)) return false;
    i = c;
    if (!is.get(c)) return false;
    num |= ((unsigned int) c) << 8;
    return true;
}

bool ctm_load_u24(unsigned int &i, istream &is) {
    unsigned char c;
    if (!is.get(c)) return false;
    i = c;
    if (!is.get(c)) return false;
    num |= ((unsigned int) c) << 8;
    if (!is.get(c)) return false;
    num |= ((unsigned int) c) << 16;
    return true;
}

bool ctm_load_u32(unsigned int &i, istream &is) {
    unsigned char c;
    if (!is.get(c)) return false;
    i = c;
    if (!is.get(c)) return false;
    num |= ((unsigned int) c) << 8;
    if (!is.get(c)) return false;
    num |= ((unsigned int) c) << 16;
    if (!is.get(c)) return false;
    num |= ((unsigned int) c) << 24;
    return true;
}

int ctm_load_action(Action &a, istream &is) {
    unsigned char c;

    do {
        if (!is.get(c)) return LoadError::le_unexpected_end;
        if (IS_MOVE(c)) {
            unsigned int num = c & 0x0f;
            PrimitiveType pt = (c & 0x10) ? pt_mover : pt_movel;
            if (num == 0x00) { // moveww
                // not implemented
            } else if (num < 0x0c) { // moveX
                for (int i = 0; i < num; i++)
                    a->primitives.emplace_back(pt);
            } else if (lower == 0x0c) { // movew1
                if(!ctm_load_u8(num, is)) return LoadError::le_unexpected_end;
                for (unsigned int i = 0; i < num; i++)
                    a->primitives.emplace_back(pt);
            } else if (lower == 0x0d) { //movew2
                if(!ctm_load_u16(num, is)) return LoadError::le_unexpected_end;
                for (unsigned int i = 0; i < num; i++)
                    a->primitives.emplace_back(pt);
            } else if (lower == 0x0e) { // movew3
                if(!ctm_load_u24(num, is)) return LoadError::le_unexpected_end;
                for (unsigned int i = 0; i < num; i++)
                    a->primitives.emplace_back(pt);
            } else { // movew4
                if(!ctm_load_u32(num, is)) return LoadError::le_unexpected_end;
                for (unsigned int i = 0; i < num; i++)
                    a->primitives.emplace_back(pt);
            }
        } else if (IS_PRINT(c)) {
            // TODO
        }
    } while()
}

int ctm_load_state(State *s, istream &is) {
}

int ctm_load(TuringMachine *tm, istream &is) {
    char c;
    // read header
    for (int i = 0; i < 3; i++) {
        if (!is.get(c)) return LoadError::le_unexpected_end;
        if (c != HEADER[i]) return LoadError::le_header;
    }
    // read version byte (not used currently)
    if (!is.get(c)) return LoadError::le_unexpected_end;

    while (is.good()) {

    }
}
