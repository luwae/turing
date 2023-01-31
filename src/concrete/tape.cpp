#include "concrete/tape.hpp"

using std::ostream;
using std::vector;

unsigned char &Tape::operator[](size_type index) {
    vector<unsigned char> *v = &right;
    if (index < 0) {
        index = ~index;
        v = &left;
    }
    if (index >= (size_type) v->size())
        v->insert(v->end(), index - (size_type) v->size() + 1, '_');
    return (*v)[index];
}

ostream &operator<<(ostream &os, const Tape &tape) {
    for (auto i = tape.left.rbegin(); i != tape.left.rend(); ++i)
        os << *i;
    for (unsigned char c : tape.right)
        os << c;
    return os;
}
