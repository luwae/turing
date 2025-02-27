use std::boxed::Box;

use chumsky::prelude::*;

#[derive(Debug, Clone)]
pub struct Machine {
    pub states: Vec<StateDesc>,
}

#[derive(Debug, Clone)]
pub struct StateDesc {
    pub name: String,
    pub branches: Vec<Branch>,
}

#[derive(Debug, Clone)]
pub struct Branch {
    pub sel: Selector,
    pub prim: Vec<Primitive>,
    pub cont: Option<Continuation>,
}

#[derive(Debug, Clone)]
pub enum Selector {
    Or(Vec<Selector>),
    And(Vec<Selector>),
    Not(Option<Box<Selector>>),
    Range(Sym, Sym),
    Elem(Sym),
}

#[derive(Debug, Clone)]
pub enum Sym {
    Imm(u8),
    Current,
}

#[derive(Debug, Clone)]
pub enum Continuation {
    Accept,
    Reject,
    State(String),
}

#[derive(Debug, Clone)]
pub enum Primitive {
    Movel,
    Mover,
    Print(Sym),
}

pub fn sym_parser() -> impl Parser<char, Sym, Error = Simple<char>> + Clone {
    let hexdigit = filter(|c: &char| c.is_ascii_hexdigit());
    let hexsym = just('x')
        .ignored()
        .then(hexdigit)
        .then(hexdigit)
        .delimited_by(just('\''), just('\''))
        .map(|((_, hi), lo)| ((hi.to_digit(16).unwrap() << 4) | lo.to_digit(16).unwrap()) as u8)
        .map(Sym::Imm);
    let normal_sym = filter(|c: &char| c.is_ascii_graphic() || *c == ' ')
        .map(|c| c as u8)
        .map(Sym::Imm)
        .delimited_by(just('\''), just('\''));

    hexsym
        .or(normal_sym)
        .or(just('$').to(Sym::Current))
        .or(just('_').to(Sym::Imm(0)))
        .padded()
}

pub fn primitive_parser() -> impl Parser<char, Primitive, Error = Simple<char>> + Clone {
    just('<')
        .to(Primitive::Movel)
        .or(just('>').to(Primitive::Mover))
        .or(just('#').ignore_then(sym_parser().map(Primitive::Print)))
        .padded()
}

pub fn selector_parser() -> impl Parser<char, Selector, Error = Simple<char>> + Clone {
    recursive(|sel| {
        let sym = sym_parser();

        // sort by decreasing precedence:
        let range = sym
            .clone()
            .then_ignore(just('.'))
            .then_ignore(just('.'))
            .then_ignore(just('='))
            .then(sym.clone())
            .map(|(lo, hi)| Selector::Range(lo, hi));

        let base = range
            .or(sym.map(Selector::Elem))
            .or(sel.delimited_by(just('('), just(')')));

        let neg = just('!')
            .repeated()
            .at_least(1)
            .then(base.clone().or_not())
            .foldr(|_op, rhs| Some(Selector::Not(rhs.map(Box::new))))
            .map(Option::unwrap)
            .or(base);

        let sand =
            neg.clone()
                .then(just('&').ignore_then(neg).repeated())
                .map(|(first, mut rest)| {
                    if rest.is_empty() {
                        first
                    } else {
                        rest.insert(0, first);
                        Selector::And(rest)
                    }
                });

        let sor = sand
            .clone()
            .then(just('|').ignore_then(sand).repeated())
            .map(|(first, mut rest)| {
                if rest.is_empty() {
                    first
                } else {
                    rest.insert(0, first);
                    Selector::Or(rest)
                }
            });

        sor.padded()
    })
}

pub fn continuation_parser() -> impl Parser<char, Continuation, Error = Simple<char>> + Clone {
    text::keyword("accept")
        .to(Continuation::Accept)
        .or(text::keyword("reject").to(Continuation::Reject))
        .or(text::ident().map(|id| Continuation::State(id)))
        .padded()
}

pub fn machine_parser() -> impl Parser<char, Machine, Error = Simple<char>> + Clone {
    let branch = selector_parser()
        .delimited_by(just('['), just(']'))
        .padded()
        .then(primitive_parser().repeated())
        .then(continuation_parser().or_not())
        .map(|((sel, prim), cont)| Branch { sel, prim, cont })
        .padded();

    let state_def = text::ident()
        .padded()
        .then(branch.repeated().delimited_by(just('{'), just('}')))
        .map(|(name, branches)| StateDesc { name, branches })
        .padded();

    state_def
        .repeated()
        .map(|states| Machine { states })
        .then_ignore(end())
}
