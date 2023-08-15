use std::collections::{HashMap, HashSet};

#[derive(Debug, Copy, Clone)]
enum Sym {
    Var(String),
    Imm(u8),
}

#[derive(Debug, Copy, Clone)]
enum Primitive {
    Movel,
    Mover,
    Print(Sym),
}

#[derive(Debug, Clone)]
enum Argument {
    Sym(Sym),
    Call(Box<Call>),
}

#[derive(Debug, Clone)]
enum Call {
    Var(String),
    Imm(String, Vec<Argument>),
    Accept,
    Reject,
}

#[derive(Debug, Copy, Clone)]
enum Selector {
    Single(Sym),
    Range(Sym, Sym),
    All,
}

#[derive(Debug, Clone)]
struct Branch {
    syms: Vec<Selector>,
    primitives: Vec<Primitive>,
    call: Box<Call>,
}

#[derive(Debug, Clone)]
enum Parameter {
    Sym(String),
    State(String),
}

#[derive(Debug, Clone)]
enum State {
    name: String,
    params: Vec<Parameter>,
    branches: Vec<Branch>
}

#[derive(Debug, Clone)]
enum Substitute {
    Sym(u8),
    Call(Box<Call>),
}
