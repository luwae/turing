#ifndef CONCRETE_TAPE_HPP
#define CONCRETE_TAPE_HPP

#include <iostream>
#include <string>
#include <vector>

class Tape {
public:
    using size_type = int;
    Tape() = default;
    Tape(const std::string &input): right(input.begin(), input.end()) { }
    size_type lsize() const { return left.size(); }
    size_type rsize() const { return right.size(); }
    size_type size() const { return left.size() + right.size(); }
    unsigned char &operator[](size_type index);
    friend std::ostream &operator<<(std::ostream &os, const Tape &tape);
private:
    std::vector<unsigned char> right;
    std::vector<unsigned char> left;
};

#endif
