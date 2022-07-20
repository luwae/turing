#include "tape.hpp"

using std::ostream;
using std::vector;

unsigned char &Tape::operator[](int index) {
    if (index >= 0) {
        if ((size_type) index >= right.size())
            right.insert(right.end(), index - (int) right.size() + 1, '_');
        return right[index];
    }
    index = -index - 1;
    if ((size_type) index >= left.size())
        left.insert(left.end(), index - (int) left.size() + 1, '_');
    return left[index];
}

ostream &operator<<(ostream &os, const Tape &tape) {
    os << "test";
    for (auto i = tape.left.rbegin(); i != tape.left.rend(); ++i)
        os << *i;
    for (unsigned char c : tape.right)
        os << c;
    return os;
}
