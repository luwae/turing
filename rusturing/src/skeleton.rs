use std::collections::{HashMap, HashSet};

enum Sym {
    Var(String),
    Real(u8),
}

impl Sym {
    fn get(&self, args: &HashMap<String, StateArg>) -> u8 {
        match self {
            Sym::Var(name) => match args.get(name) {
                Some(StateArg::Sym(Sym::Real(val))) => *val,
                _ => panic!("free var"),
            }
            Sym::Real(val) => *val,
        }
    }
}

enum StateArg {
    Sym(Sym),
    // Call(Call),
}

/*
enum RelativeCall {
    Global(String, Vec<StateArg>), // args can be empty
    Local(usize, String, Vec<StateArg>), // name and args can be empty
}

// vec is enough indirection here?

enum Call {
    Accept,
    Reject,
    Var(String),
    Real(RelativeCall),
    Anon(Box<State>),
}

enum Action {
    Mover,
    Movel,
    Print(Sym),
    Call(Call),
}
*/

enum Selector {
    Empty,
    Sym(Sym),
    Range(Sym, Sym),
    List(Vec<Selector>),
    Invert(Box<Selector>),
}

impl Selector {
    fn contains(&self, sym: u8, args: &HashMap<String, StateArg>) -> bool {
        match self {
            Selector::Empty => false,
            Selector::Sym(asym) => asym.get(args) == sym,
            Selector::Range(start, end) => start.get(args) <= sym && sym <= end.get(args),
            Selector::List(sels) => sels.iter().any(|sel| sel.contains(sym, args)),
            Selector::Invert(sel) => !sel.contains(sym, args),
        }
    }
}

/*
struct Branch {
    sel: Selector,
    actions: Vec<Action>,
}

struct State {
    name: String,
    args: Vec<String>,
    branches: Vec<Branch>,

    children: RefCell<Vec<Rc<State>>>,
    parent: Weak<State>,
}

struct Machine {
    states: Vec<State>,
}
*/

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_selector() {
        let sel_false = Selector::Empty;
        let sel_true = Selector::Invert(Box::new(Selector::Empty));
        let sel_42 = Selector::Sym(Sym::Real(42));
        let sel_var = Selector::Sym(Sym::Var("a".to_string()));
        let sel_invrange = Selector::List(vec![
            Selector::Sym(Sym::Real(105)),
            Selector::Invert(Box::new(Selector::Range(
                Sym::Real(100),
                Sym::Real(110),
            )))
        ]);
        let mut args = HashMap::new();
        args.insert("a".to_string(), StateArg::Sym(Sym::Real(42)));
        for sym in 0..=255 {
            assert!(!sel_false.contains(sym, &args));
            assert!(sel_true.contains(sym, &args));
            assert_eq!(sym == 42, sel_42.contains(sym, &args));
            assert_eq!(sym == 42, sel_var.contains(sym, &args));
            assert_eq!(sym < 100 || sym == 105 || sym > 110, sel_invrange.contains(sym, &args));
        }
    }

    #[test]
    #[should_panic]
    fn test_selector_panic() {
        let sel = Selector::Sym(Sym::Var("a".to_string()));
        let args = HashMap::new();
        sel.contains(42, &args);
    }
}
