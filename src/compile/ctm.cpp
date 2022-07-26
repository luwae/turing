#include "machine.hpp"

#define IS_GOTO(b) (((b) & 0x80) == 0x80)
#define IS_PRINT(b) (((b) & 0xc0) == 0x40)
#define IS_MOVE(b) (((b) & 0xe0) == 0x20)
#define IS_CHAR(b) (((b) & 0xf0) == 0x10)

#define OP_STATE 0x10
#define OP_CHAR_RESERVED 0x1c
#define OP_CHAR_RANGE 0x1d
#define OP_CHAR_DEFLT 0x1e
#define OP_CHAR_W1 0x1f

#define ERR_LOAD_EXPECTED_STATE -1
#define ERR_END -2

#define AT(idx) if (idx >= sz) return ERR_END

class MachineLoader {
public:
    MachineLoader(TuringMachine *tm, const unsigned char *data, size_t sz):
        tm(tm), data(data), sz(sz) { load(); }
private:
    unsigned char at(size_t n) { 
        if (n >= off) throw runtime_error("unexpected end");
        return data[n];
    }
    void load();
    void load_state();

    TuringMachine * const tm;
    const unsigned char * const data;
    size_t off = 0;
    size_t sz;
};

void MachineLoader::load_state() {
    unsigned char d = at(off);

    Branch b;
    // TODO loop, but at least once
    if (IS_CHAR(d)) {
        switch (d) {
        case OP_STATE:
        case OP_CHAR_RESERVED:
            throw runtime_error("expected char");
        case OP_CHAR_RANGE:
            unsigned char start = at(off + 2);
            unsigned char size = at(off + 1);
            if (start + (int) size > 255)
                throw runtime_error("range overflow");
            for (size_t i = start; i < start + size; i++)
                b.chars.insert(c);
            break;
        case OP_CHAR_DEFLT:
            // noting; check for empty branch later
            break;
        case OP_CHAR_W1:
            unsigned char size = at(off + 1);
            if (size == 0)
                throw runtime_error("empty char");
            for (size_t i = off + 2; i < off + 2 + size; i++)
                b.chars.insert(at(i));
            break;
        default:
            size_t size = d & 0x0f;
            for (size_t i = off + 1; i < off + 1 + size; i++)
                b.chars.insert(at(i));
        }
    } else {
        throw runtime_error("expected char");
    }
}

void MachineLoader::load() {
    while (off < sz) {
        if (data[off++] != OP_STATE)
            throw runtime_error("expected state definition");
        load_state();
    }
    return 0;
}
