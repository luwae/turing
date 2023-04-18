pub enum Sym {
    Concrete(u8),
    Abstract(String),
}

pub enum Primitive {
    Movel,
    Mover,
    Print(Sym),
}

pub enum CallArg {
    Sym(Sym),
    Call(Call),
}

pub struct Call {
    name: String,
    args: Vec<CallArg>,
}

pub enum Transition {
    Continue(Call),
    Accept,
    Reject,
}

pub enum SelectorArg {
    Single(Sym),
    Range(Sym, Sym),
}

pub enum Selector {
    Deflt,
    Syms(Vec<SelectorArg>),
}

pub struct Branch {
    selector: Selector,
    primitives: Vec<Primitive>,
    trans: Transition,
}

pub enum StateArg {
    State(String),
    Sym(String),
}

pub struct State {
    name: String,
    args: Vec<StateArg>,
    branches: Vec<Branch>,
}

pub struct Machine {
    states: Vec<State>,
}
