# turing
Tools to Design and Run Turing Machines

The tools and machines in this repository have no practical relevance, they are merely created for fun and to use them in other projects. The design uses a variant of **Skeleton Tables** from Turing's original 1936 paper, which can be expanded trivially to form concrete turing machines. For more information on the design, check the **DESIGN** files in the machines/ directory.

features (complete):
 * abstract turing machines
    + language grammar
    + source-to-source compiler
 * concrete turing machines
    + language grammar
    + interpreter

features (planned):
 * compressed turing machine file format

machines (complete):
 * calculate pi to arbitrary precision

machines (planned):
 * calculate e to arbitrary precison
 * 8-bit stack machine

[Grammar of concrete turing machines (.tm files)](grammar_concrete.pdf)

[Grammar of abstract turing machines (.atm files)](grammar_abstract.pdf)

**NOTE** This project is an exercise in programming as well as lexer/parser/compiler writing. This is why I implemented everything myself instead of using external tools like lex and bison.
**NOTE** This project is currently being rewritten in Rust. See rusturing/ directury.
