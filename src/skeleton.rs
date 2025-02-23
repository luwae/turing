use std::boxed::Box;

use chumsky::prelude::*;

#[derive(Debug, Clone)]
pub struct Machine {
    states: Vec<StateDef>,
}

#[derive(Debug, Clone)]
pub struct StateDef {
    pub name: String,
    pub params: Vec<String>,
    pub branches: Vec<Branch>,
}

#[derive(Debug, Clone)]
pub struct Branch {
    pub sel: Selector,
    pub chain: Chain,
}

#[derive(Debug, Clone)]
pub enum Selector {
    Or(Vec<Selector>),
    And(Vec<Selector>),
    Not(Box<Option<Selector>>),
    Range(SelectorElem, SelectorElem),
    Elem(SelectorElem),
}
#[derive(Debug, Clone)]
pub enum SelectorElem {
    Sym(u8),
    Id(String),
}

#[derive(Debug, Clone)]
pub struct Chain {
    pub parts: Vec<ChainElem>,
    pub term: Option<Terminate>,
}

#[derive(Debug, Clone)]
pub enum ChainElem {
    Prim(Primitive),
    Call { id: String, args: Vec<CallArg> },
}

#[derive(Debug, Clone)]
pub enum Primitive {
    Movel,
    Mover,
    Print(SelectorElem),
}

#[derive(Debug, Clone)]
pub enum Terminate {
    Accept,
    Reject,
}

#[derive(Debug, Clone)]
pub enum CallArg {
    Sel(Selector),
    Chain(Chain),
    Sym(u8),
    Id(String),
}

pub fn sym_parser() -> impl Parser<char, u8, Error = Simple<char>> {
    let hexdigit = filter(|c: &char| c.is_ascii_hexdigit());
    let hexsym = just('x')
        .ignored()
        .then(hexdigit)
        .then(hexdigit)
        .delimited_by(just('\''), just('\''))
        .map(|((_, hi), lo)| ((hi.to_digit(16).unwrap() << 4) | lo.to_digit(16).unwrap()) as u8);
    let normal_sym = filter(|c: &char| c.is_ascii_graphic() || *c == ' ')
        .map(|c| c as u8)
        .delimited_by(just('\''), just('\''));

    hexsym.or(normal_sym).padded()
}

pub fn selector_elem_parser() -> impl Parser<char, SelectorElem, Error = Simple<char>> {
    use SelectorElem as SE;
    sym_parser()
        .map(SE::Sym)
        .or(text::ident().map(SE::Id))
        .padded()
}

pub fn primitive_parser() -> impl Parser<char, Primitive, Error = Simple<char>> {
    just('<')
        .map(|_| Primitive::Movel)
        .or(just('>').map(|_| Primitive::Mover))
        .or(selector_elem_parser().map(Primitive::Print))
        .padded()
}

pub fn selector_parser() -> impl Parser<char, Selector, Error = Simple<char>> {
    recursive(|sel| {
        let elem = selector_elem_parser().or(sel.delimited_by(just('('), just(')')));

        // sort by decreasing precedence:
        let range = elem.clone().then(just('.')).then(just('.')).then(elem);

        let neg = just('!').repeated().then(range.or_not())
            .foldr(todo!());

        let and = neg.clone()
            .then(just('&').then(neg).repeated())
            .foldl(todo!());

        let or = and.clone()
            .then(just('|').then(and).repeated())
            .foldl(todo!());

        or
    })
}

pub fn chain_parser() -> impl Parser<char, Chain, Error = Simple<char>> {
    recursive(|chain| {
        let call = text::ident().padded().then(
            text::ident()
            .then(just(',').then(text::ident()).repeated().collect::<Vec<_>>())
            .delimited_by(just('('), just(')'))
            .or_not());
        let chain_elem = primitive_parser().or(call)
    })
}
