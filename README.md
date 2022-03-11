# turing
A turing machine simulation written in C.

This is written as a just for fun project to practice writing lexers, compilers, and inventing file formats.
Following features are (planned to be) included:
 * simple programming language for specifying turing machines
    + arbitrary amount of states
    + multi-character or wildcard actions in states
    + recursive anonymous state definitions with arbitrary depth
 * simple binary file format for compiled turing machines
    + efficient binary storage
    + faster execution than interpreted
    + able to "link" several turing machines together
 * compiler + runtime
 * interpreter
