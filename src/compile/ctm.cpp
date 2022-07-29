#include <iostream>
#include <string>

#include "ctm.hpp"

#define IS_GOTO(b) (((b) & 0x80) == 0x80)
#define IS_PRINT(b) (((b) & 0xc0) == 0x40)
#define IS_PRINT_NO_VAR(b) (((b) & 0xe0) == 0x40)
#define IS_MOVE(b) (((b) & 0xe0) == 0x20)
#define IS_CHAR(b) (((b) & 0xf0) == 0x10)

using std::istream;
using std::string;

bool get_byte(unsigned char &c, istream &is) {
    char sc;
    if (!is.get(sc))
        return false;
    c = (unsigned int) sc;
    return true;
}

bool ctm_decode_int(unsigned int &num, int nb, istream &is) {
    unsigned char c;
    num = 0;
    for (int i = 0; i < nb; i++) {
        if (!get_byte(c, is)) return DECODE_END;
        num |= ((unsigned int) c) << (i << 3);
    }
    return true;
}

int ctm_decode_branchbody(Branch &b, unsigned int this_index, istream &is) {
    unsigned char c;
    bool first_iter = true;

    // default values for optional goto
    b.action.term = TerminateType::term_cont;
    b.action.next = this_index;

    while (true) {
        if (!first_iter) {
            if (is.peek() == EOF || is.peek() == 0x10
                    || is.peek() == 0x1c || is.peek() == 0x1d || is.peek() == 0x1f) // TODO IS_CHAR?
                return 0;
        }
        if (!get_byte(c, is)) return DECODE_END;
        if (IS_GOTO(c)) {
            TerminateType term = TerminateType::term_cont;
            unsigned int num = c & 0x7f;
            if (num < 0x78) { // gotoX
                // num already loaded
            } else if (num < 0x7c) { // gotowX
                if (!ctm_decode_int(num, num & 0x03, is)) return DECODE_END;
            } else if (num == 0x7c) { // goto stay
                num = this_index;
            } else if (num == 0x7d) { // goto acc
                term = TerminateType::term_acc;
            } else if (num == 0x7e) { // goto rej
                term = TerminateType::term_rej;
            } else { // gotoww
                return DECODE_NOT_IMPLEMENTED;
            }
            b.action.term = term;
            b.action.next = num;
            return 0; // body ends after goto
        } else if (IS_PRINT_NO_VAR(c)) { // print (without var)
            unsigned char sym;
            if (c & 0x10) return DECODE_NOT_IMPLEMENTED;
            if (c & 0x01)
                b.action.primitives.emplace_back(PrimitiveType::pt_movel);
            if (c & 0x02)
                b.action.primitives.emplace_back(PrimitiveType::pt_mover);
            if (!get_byte(sym, is)) return DECODE_END;
            b.action.primitives.emplace_back(PrimitiveType::pt_print, sym);
            if (c & 0x04)
                b.action.primitives.emplace_back(PrimitiveType::pt_movel);
            if (c & 0x08)
                b.action.primitives.emplace_back(PrimitiveType::pt_mover);
        } else if (IS_PRINT(c)) { // print (with var)
            PrimitiveType move = (c & 0x10) ? pt_mover : pt_movel;
            unsigned int num = c & 0x0f;
            unsigned char sym;
            if (num == 0) { // printwwl/r
                return DECODE_NOT_IMPLEMENTED;
            } else if (num < 0x0c) {
                // num already loaded
            } else { // gotowXl/r
                if (!ctm_decode_int(num, num & 0x03, is)) return DECODE_END;
            }
            for (unsigned int i = 0; i < num; i++) {
                if (!get_byte(sym, is)) return DECODE_END;
                b.action.primitives.emplace_back(PrimitiveType::pt_print, sym);
                if (i != num - 1)
                    b.action.primitives.emplace_back(move);
            }
        } else if (IS_MOVE(c)) { // move
            PrimitiveType move = (c & 0x10) ? pt_mover : pt_movel;
            unsigned int num = c & 0x0f;
            if (num == 0) {
                return DECODE_NOT_IMPLEMENTED;
            } else if (num < 0x0c) {
                // num already loaded
            } else {
                if (!ctm_decode_int(num, num & 0x03, is)) return DECODE_END;
            }
            for (unsigned int i = 0; i < num; i++)
                b.action.primitives.emplace_back(move);
        } else {
            return DECODE_EXPECTED_BRANCHBODY;
        }

        first_iter = false;
    }
}

int ctm_decode_branchspec(Branch &b, istream &is) {
    unsigned char c;
    bool first_iter = true;

    while (true) {
        if (!first_iter) {
            if (IS_PRINT(is.peek()) || IS_MOVE(is.peek()) || IS_GOTO(is.peek()))
                return 0;
        }

        if (!get_byte(c, is)) return DECODE_END;
        if (!IS_CHAR(c)) return DECODE_EXPECTED_BRANCHSPEC;
        unsigned int num = c & 0x0f;
        if (num == 0x00) { // state
            return DECODE_EXPECTED_BRANCHSPEC;
        } else if (num < 0x0c) { // charX
            for (int i = 0; i < num; i++) {
                if (!get_byte(c, is)) return DECODE_END;
                if (!b.charset_invert)
                    b.chars.insert(c);
            }
        } else if (num == 0x0c) { // charw1
            if (!ctm_decode_int(num, 1, is)) return DECODE_END;
            for (int i = 0; i < num; i++) {
                if (!get_byte(c, is)) return DECODE_END;
                if (!b.charset_invert)
                    b.chars.insert(c);
            }
        } else if (num == 0x0d) { // charrange
            unsigned char size, start;
            if (!get_byte(size, is)) return DECODE_END;
            if (!get_byte(start, is)) return DECODE_END;
            if (size == 0 || size + (unsigned int) start > 256) return DECODE_INVL_RANGE;
            if (!b.charset_invert)
                for (unsigned char i = 0; i < size; i++)
                    b.chars.insert(start + i);
        } else if (num == 0x0e) { // reserved
            return DECODE_NOT_IMPLEMENTED;
        } else { // charall
            b.charset_invert = true;
            b.chars.clear();
        }

        first_iter = false;
    }
}

string ctm_statename(unsigned int index) {
    return "s" + std::to_string(index);
}

int ctm_decode_state(TuringMachine &tm, istream &is) {
    unsigned char c;
    int status;
    State s;
    s.name = ctm_statename(tm.size());

    while (true) {
        if (is.peek() == EOF || is.peek() == 0x10) {
            tm.add_state(s);
            return 0;
        } else {
            Branch b;
            status = ctm_decode_branchspec(b, is);
            if (status) return status;
            status = ctm_decode_branchbody(b, tm.size(), is);
            if (status) return status;
            s.branches.push_back(b);
        }
    }
}

int ctm_decode_machine(TuringMachine &tm, istream &is) {
    unsigned char c;
    int status;
    bool first_iter = true;

    while (true) {
        if (!first_iter) {
            if (is.peek() == EOF)
                return 0;
        }
        if (!get_byte(c, is)) return DECODE_END;
        if (c != 0x10) return DECODE_EXPECTED_STATE;
        status = ctm_decode_state(tm, is);
        if (status) return status;

        first_iter = false;
    }
}

const unsigned char HEADER[] = {'C', 'T', 'M'};

int ctm_decode(TuringMachine &tm, istream &is) {
    unsigned char c;
    for (int i = 0; i < 3; i++) { // read magic number
        if (!get_byte(c, is)) return DECODE_END;
        if (c != HEADER[i]) return DECODE_HEADER;
    }
    if (!get_byte(c, is)) return DECODE_END; // version byte (ignored for now)

    return ctm_decode_machine(tm, is);
}
