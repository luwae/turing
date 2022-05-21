#ifndef TAPE_HPP
#define TAPE_HPP

#include <iostream>
#include <string>
#include <vector>

class Tape {
public:
    friend std::ostream &operator<<(std::ostream &os, const Tape &t);
    Tape() = default;
    Tape(const std::string &input): right(input.begin(), input.end()) { }
    Tape(const std::vector<unsigned char> &v): right(v.begin(), v.end()) { }
    unsigned char &operator[](int index);
    int size() { return right.size() + left.size(); }
    int rsize() { return right.size(); }
    int lsize() { return left.size(); }
private:
    std::vector<unsigned char> right;
    std::vector<unsigned char> left;
};

#endif
