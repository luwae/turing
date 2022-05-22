#include "tape.hpp"

using std::ostream;
using std::vector;

unsigned char &Tape::operator[](int index) {
    if (index >= 0) {
        if ((vector<unsigned char>::size_type) index >= right.size())
            right.insert(right.end(), index - right.size() + 1, '_');
        return right[index];
    }
    index = -index - 1;
    if ((vector<unsigned char>::size_type) index >= left.size())
        left.insert(left.end(), index - left.size() + 1, '_');
    return left[index];
}

ostream &operator<<(ostream &os, const Tape &t) {
    for (auto i = t.left.rbegin(); i != t.left.rend(); ++i)
        os << *i;
    for (unsigned char c : t.right)
        os << c;
    return os;
}
