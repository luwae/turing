use std::boxed::Box;

use crate::exec;

use chumsky::prelude::*;

#[derive(Debug, Clone)]
pub struct Machine {
    pub objects: Vec<TopLevelObject>,
}

#[derive(Debug, Clone)]
pub enum TopLevelObject {
    State(StateDesc),
    Map(SymMap),
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
    Context,
    Mapped(String, Box<Sym>),
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

#[derive(Debug, Clone)]
pub struct SymMap {
    name: String,
    branches: Vec<(Selector, Sym)>,
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

    recursive(|sym| {
        hexsym
            .or(normal_sym)
            .or(just('_').to(Sym::Imm(0)))
            .or(just('$').to(Sym::Context))
            .or(text::ident()
                .then(sym.delimited_by(just('('), just(')')))
                .map(|(name, arg)| Sym::Mapped(name, Box::new(arg))))
            .padded()
    })
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
        .or(text::ident().map(Continuation::State))
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

    let statedesc = text::ident()
        .padded()
        .then(
            branch
                .repeated()
                .delimited_by(just('{'), just('}'))
                .or(primitive_parser()
                    .repeated()
                    .then(continuation_parser().or_not())
                    .map(|(prim, cont)| {
                        vec![Branch {
                            sel: Selector::Not(None),
                            prim,
                            cont,
                        }]
                    })
                    .delimited_by(just('{'), just('}'))),
        )
        .map(|(name, branches)| StateDesc { name, branches })
        .padded();

    let symmap = text::keyword("map")
        .padded()
        .ignore_then(text::ident())
        .padded()
        .then(
            selector_parser()
                .delimited_by(just('['), just(']'))
                .padded()
                .then(sym_parser())
                .padded()
                .repeated()
                .delimited_by(just('{'), just('}')),
        )
        .map(|(name, branches)| SymMap { name, branches })
        .padded();

    let tlo = symmap
        .map(TopLevelObject::Map)
        .or(statedesc.map(TopLevelObject::State));

    tlo.repeated()
        .map(|objects| Machine { objects })
        .then_ignore(end())
}

impl SymMap {
    fn apply(&self, sym: u8, machine: &Machine, context: u8) -> u8 {
        for branch in &self.branches {
            if branch.0.matches(sym, machine, context) {
                return branch.1.real(machine, context);
            }
        }
        panic!("sym {} not found in map {}", sym, self.name);
    }
}

impl Sym {
    fn real(&self, machine: &Machine, context: u8) -> u8 {
        match self {
            Self::Imm(i) => *i,
            Self::Context => context,
            Self::Mapped(name, sym) => {
                let real_sym = sym.real(machine, context);
                match machine.map_by_name(name) {
                    Some(map) => map.apply(real_sym, machine, context),
                    None => panic!("no map named {}", name),
                }
            }
        }
    }
}

impl Selector {
    fn matches(&self, sym: u8, machine: &Machine, context: u8) -> bool {
        match self {
            Self::Or(ss) => ss.iter().any(|sel| sel.matches(sym, machine, context)),
            Self::And(ss) => ss.iter().all(|sel| sel.matches(sym, machine, context)),
            Self::Not(maybe_inner) => match maybe_inner {
                Some(inner) => !inner.matches(sym, machine, context),
                None => true,
            },
            Self::Range(a, b) => {
                let real_a = a.real(machine, context);
                let real_b = b.real(machine, context);
                sym >= real_a && sym <= real_b
            }
            Self::Elem(s) => {
                if let Sym::Context = s {
                    panic!("selector contains context");
                }
                s.real(machine, context) == sym
            }
        }
    }
}

impl StateDesc {
    fn matching_branch(&self, sym: u8, machine: &Machine, context: u8) -> Option<&Branch> {
        self.branches
            .iter()
            .find(|b| b.sel.matches(sym, machine, context))
    }
}

impl Machine {
    fn states(&self) -> impl Iterator<Item = &StateDesc> {
        self.objects.iter().filter_map(|obj| match obj {
            TopLevelObject::State(s) => Some(s),
            TopLevelObject::Map(_) => None,
        })
    }

    fn maps(&self) -> impl Iterator<Item = &SymMap> {
        self.objects.iter().filter_map(|obj| match obj {
            TopLevelObject::State(_) => None,
            TopLevelObject::Map(m) => Some(m),
        })
    }

    fn state_by_name(&self, state_name: &str) -> Option<&StateDesc> {
        self.states().find(|s| s.name == state_name)
    }

    fn state_by_idx(&self, state_idx: usize) -> Option<&StateDesc> {
        self.states().nth(state_idx)
    }

    fn state_name_to_idx(&self, state_name: &str) -> Option<usize> {
        self.states().position(|s| s.name == state_name)
    }

    fn map_by_name(&self, map_name: &str) -> Option<&SymMap> {
        self.maps().find(|m| m.name == map_name)
    }
}

impl exec::Machine for Machine {
    fn primitives(
        &self,
        state_idx: usize,
        sym: u8,
    ) -> Option<impl Iterator<Item = exec::Primitive>> {
        if let Some(s) = self.state_by_idx(state_idx) {
            s.matching_branch(sym, self, sym).map(|b| {
                b.prim.iter().map(move |p| match p {
                    Primitive::Movel => exec::Primitive::Movel,
                    Primitive::Mover => exec::Primitive::Mover,
                    Primitive::Print(s) => exec::Primitive::Print(s.real(self, sym)),
                })
            })
        } else {
            panic!("invalid state index {}", state_idx);
        }
    }

    fn continuation(&self, state_idx: usize, sym: u8) -> exec::Continuation {
        if let Some(s) = self.state_by_idx(state_idx) {
            if let Some(b) = s.matching_branch(sym, self, sym) {
                match &b.cont {
                    Some(Continuation::Accept) => exec::Continuation::Accept,
                    Some(Continuation::Reject) => exec::Continuation::Reject,
                    Some(Continuation::State(name)) => {
                        if let Some(cont_state_idx) = self.state_name_to_idx(name) {
                            exec::Continuation::State(cont_state_idx)
                        } else {
                            panic!("invalid state name {}", name)
                        }
                    }
                    None => exec::Continuation::State(state_idx),
                }
            } else {
                exec::Continuation::Reject
            }
        } else {
            panic!("invalid state index {}", state_idx);
        }
    }
}
