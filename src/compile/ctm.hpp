#ifndef TURING_CTM_HPP
#define TURING_CTM_HPP

#include <iostream>

#include "machine.hpp"

#define DECODE_END 1
#define DECODE_HEADER 2
#define DECODE_EXPECTED_STATE 3
#define DECODE_INVL_RANGE 4
#define DECODE_EXPECTED_BRANCHSPEC 5
#define DECODE_EXPECTED_BRANCHBODY 6
#define DECODE_NOT_IMPLEMENTED 7

int ctm_decode(TuringMachine &tm, std::istream &is);

#endif
