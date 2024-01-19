pub enum Primitive {
    Movel,
    Mover,
    Print(u8),
}

pub enum Call {
    StateIdx(usize),
    Accept,
    Reject,
}

pub enum Selector {
    Single(u8),
    RangeInclusive(u8, u8),
    All,
}

impl Selector {
    pub fn matches(&self, sym: u8) -> bool {
        match self {
            Selector::Single(sel_sym) => sym == *sel_sym,
            Selector::RangeInclusive(sel_start, sel_end) => sym >= *sel_start && sym <= *sel_end,
            Selector::All => true,
        }
    }
}

pub struct Branch {
    pub syms: Vec<Selector>,
    pub primitives: Vec<Primitive>,
    pub call: Call,
}

pub struct State {
    pub name: String,
    pub branches: Vec<Branch>,
}

pub struct TuringMachine {
    pub states: Vec<State>,
}
