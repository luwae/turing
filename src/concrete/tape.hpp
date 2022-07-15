#ifndef TAPE_HPP
#define TAPE_HPP

#include <iostream>
#include <string>
#include <vector>

class Tape {
public:
    using size_type = std::vector<unsigned char>::size_type;
    Tape() = default;
    Tape(const std::string &input): right(input.begin(), input.end()) { }
    Tape(const std::vector<unsigned char> &v): right(v.begin(), v.end()) { }
    unsigned char &operator[](int index);
    size_type size() const { return right.size() + left.size(); }
    size_type rsize() const { return right.size(); }
    size_type lsize() const { return left.size(); }
    friend std::ostream &operator<<(std::ostream &os, const Tape &tape);
private:
    std::vector<unsigned char> right;
    std::vector<unsigned char> left;
};

#endif
