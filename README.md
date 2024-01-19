# turing
Tools to Design and Run Turing Machines

The tools and machines in this repository have no practical relevance, they are merely created for fun and to use them in other projects. The design uses a variant of **Skeleton Tables** from Turing's original 1936 paper, which can be expanded trivially to form concrete turing machines. For more information on the design, check the **DESIGN** files.

**NOTE**: The main branch contains the last working version implemented in C++. It's not very well documented. I plan on doing a Rust version eventually.

## Features
 * abstract turing machines
    + language grammar
    + expansion to concrete turing machines (atm2tm)
 * concrete turing machines
    + language grammar
    + interpreter (tmrun)

## Machines
 * calculate pi to arbitrary precision
