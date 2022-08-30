#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <set>

#include "parse.hpp"

using std::cout; using std::endl;
using std::ifstream; using std::stringstream;
using parse::Parser;
using std::string;
using std::unique_ptr; using std::make_unique;
using std::set;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "please enter filename" << endl;
        return -1;
    }
    ifstream file(argv[1]);
    if (file.fail()) {
        cout << "cannot open file" << endl;
        return -1;
    }
    stringstream ss;
    ss << file.rdbuf();
    string input = ss.str();

    cout << "file contents:\n";
    cout << input << endl;
   
    Parser ps(input);
}
