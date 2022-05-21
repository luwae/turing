#include "tape.hpp"

using std::ostream;

unsigned char &Tape::operator[](int index) {
    if (index >= 0) {
        if (index >= right.size())
            right.insert(right.end(), index - right.size() + 1, '_');
        return right[index];
    }
    index = -index - 1;
    if (index >= left.size())
        left.insert(left.end(), index - left.size() + 1, '_');
    return left[index];
}

/*
unsigned char &Tape::operator[](int index) {
    std::vector<unsigned char> &v = (index >= 0) ? right : left;
    if (index < 0)
        index = -index - 1;
    if (index >= v.size())
        v.insert(v.end(), index - v.size() + 1, '_');
    return v[index];
}
*/

ostream &operator<<(ostream &os, const Tape &t) {
    for (auto i = t.left.rbegin(); i != t.left.rend(); ++i)
        cout << *i;
    for (unsigned char c : t.right)
        cout << c;
    return os;
}
