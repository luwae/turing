use std::collections::{HashMap, HashSet};

enum SubError {
    ExpectedSym,
    ExpectedCall,
    Unresolved(String),
}

#[derive(Debug, Copy, Clone)]
enum Sym {
    Var(String),
    Imm(u8),
}

impl Sym {
    fn applied_string(&self, rsv: &mut Resolver) -> String {
        match self {
            Sym::Var(name) => match bindings.get(name) {
                Some(Substitute::Sym(val)) => Ok(format!("'x{:02x}'", val)),
                Some(_) => Err(SubError::ExpectedSym),
                None => Err(SubError::Unresolved(name.clone())),
            },
            Sym::Imm(val) => Ok(format!("'x{:02x}'", val)),
        }
    }
}

#[derive(Debug, Copy, Clone)]
enum Primitive {
    Movel,
    Mover,
    Print(Sym),
}

impl Primitive {
    fn applied_string(&self, bindings: &HashMap<String, Substitute>) -> Result<String, SubError> {
        Ok(match self {
            Primitive::Movel => "<".to_string()),
            Primitive::Mover => ">".to_string()),
            Primitive::Print(sym) => format!("={}", sym.applied_string(bindings)?),
        })
    }
}

#[derive(Debug, Clone)]
enum Argument {
    Sym(u8),
    Call(Box<Call>),
}

impl Argument {
    fn applied_string(&self, bindings: &HashMap<String, Substitute>) -> Result<String, SubError> {
        match self {
            Argument::Sym(sym) => sym.applied_string(bindings),
            Argument::Call(call) => call.applied_string(bindings),
        }
    }
}

struct NamedCall {
    name: String,
    args: Vec<Argument>,
}

#[derive(Debug, Clone)]
enum Call {
    // NOTE: this could be var or imm!
    RealCall(String, Vec<Argument>),
    Var(String),
    Accept,
    Reject,
}

impl Call {
    fn applied_string(&self, bindings: &HashMap<String, Substitute>) -> Result<String, SubError> {
        Ok(match self {
            Call::Call(name, args) => {
                let mut s = name.clone();
                if args.len() > 0 {
                    s.push('(');
                    for arg in args {
                        s.push_str(arg.applied_string(bindings)?.as_str());
                        s.push(',');
                    }
                    s.pop();
                    s.push(')');
                }
                s
            },
            Call::Accept => "accept".to_string(),
            Call::Reject => "reject".to_string(),
        })
    }
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

impl Parameter {
    fn name(&self) -> &String {
        match self {
            Parameter::Sym(s) => s,
            Parameter::State(s) => s,
        }
    }
}

#[derive(Debug, Clone)]
enum State {
    name: String,
    params: Vec<Parameter>,
    branches: Vec<Branch>
}

impl State {
    fn expanded_name(&self, rsv: &mut Resolver) -> Result<String, SubError> {
        if self.params.len() != self.args.len() {
            panic!("wrong number of arguments for state \"{}\"", self.name);
        }
        let mut s = self.name.clone();
        if self.params > 0 {
            s.push('(');
            for maybe_arg in self.params.iter().map(|p| rsv.bindings.get(p.name())) {
                match maybe_arg {
                    Some(arg) => { s.push_str(arg.expanded_name().to_str()); },
                    None => { return Err(SubError::UnresolvedString(TODO)); },
                }
            }
            for arg in args {
                s.push_str(arg.expanded_name().to_str());
                s.push(',');
            }
            s.pop();
            s.push(')');
            s
        }
    }

    fn applied_string(&self, rsv: &mut Resolver) -> Result<String, SubError> {
        let mut exp = self.expanded_name();
        done.insert()
    }
}

struct Resolver {
    states: Vec<State>,
    bindings: Vec<HashMap<String, Argument>>,
    done: HashSet<String>,
    todo: Vec<NamedCall>,
}

impl Resolver {
    fn insert_todo(&mut self, name: String, args: Vec<Argument>) {
        let state = self.states.iter().find(|s| s.name == name).expect("no state named '{}'", name);
        let exp = state.expanded_name(args.as_slice());
        if !self.done.contains(&exp) {
            self.todo.push((name, args));
        }
    }
}

fn resolve(states: Vec<State>) {
    let mut resolver = Resolver {
        states,
        bindings: Vec::new(),
        done: HashSet::new(),
        todo: Vec::new(),
    }
}
