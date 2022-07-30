#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "machine.hpp"
#include "parse.hpp"

using std::cout; using std::endl;
using std::ifstream;
using std::stringstream;
using parse::Parser;
using std::string;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "usage: parse_test <machine.tm>" << endl;
        return -1;
    }
    ifstream in(argv[1]);
    stringstream buffer;
    buffer << in.rdbuf();

    TuringMachine tm;
    Parser parser(buffer.str(), tm);

    cout << tm << endl;
}
