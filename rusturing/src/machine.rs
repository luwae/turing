use crate::tape::Tape;
use std::collections::HashSet;
use std::rc::Rc;

#[derive(Debug, Copy, Clone)]
pub enum Primitive {
    Movel,
    Mover,
    Print(u8),
}

#[derive(Debug, Copy, Clone)]
pub enum Transition {
    Continue(usize),
    Accept,
    Reject,
}

pub struct Branch {
    deflt: bool,
    syms: HashSet<u8>,
    primitives: Vec<Primitive>,
    trans: Transition,
}

impl Branch {
    pub fn new() -> Self {
        Branch {
            deflt: false,
            syms: HashSet::new(),
            primitives: Vec::new(),
            trans: Transition::Reject,
        }
    }

    pub fn set_deflt(&mut self, deflt: bool) {
        self.deflt = deflt;
    }

    pub fn add_sym(&mut self, sym: u8) {
        self.syms.insert(sym);
    }

    pub fn add_primitive(&mut self, primitive: Primitive) {
        self.primitives.push(primitive);
    }

    pub fn get_primitives(&self) -> &Vec<Primitive> {
        &self.primitives
    }

    pub fn set_trans(&mut self, trans: Transition) {
        self.trans = trans;
    }
    
    pub fn get_trans(&self) -> Transition {
        self.trans
    }

    pub fn contains(&self, sym: u8) -> bool {
        self.deflt || self.syms.contains(&sym)
    }
}

pub struct State {
    name: String,
    branches: Vec<Branch>,
}

impl State {
    pub fn new(name: String) -> Self {
        State {
            name,
            branches: Vec::new(),
        }
    }

    pub fn push(&mut self, branch: Branch) {
        self.branches.push(branch);
    }

    pub fn get_by_sym(&self, sym: u8) -> Option<&Branch> {
        self.branches.iter().find(|branch| branch.contains(sym))
    }
    
    pub fn get_by_idx(&self, idx: usize) -> Option<&Branch> {
        self.branches.get(idx)
    }
    
    pub fn get_by_idx_mut(&mut self, idx: usize) -> Option<&mut Branch> {
        self.branches.get_mut(idx)
    }

    pub fn get_name(&self) -> &str {
        &self.name
    }

    pub fn len(&self) -> usize {
        self.branches.len()
    }
}

pub struct Machine {
    states: Vec<State>,
}

impl Machine {
    pub fn new() -> Self {
        Machine {
            states: Vec::new(),
        }
    }

    pub fn push(&mut self, state: State) {
        self.states.push(state);
    }
    
    pub fn get_by_idx(&self, idx: usize) -> Option<&State> {
        self.states.get(idx)
    }
    
    pub fn get_by_idx_mut(&mut self, idx: usize) -> Option<&mut State> {
        self.states.get_mut(idx)
    }
    
    pub fn len(&self) -> usize {
        self.states.len()
    }
}

pub fn add_one() -> Machine {
    let mut machine = Machine::new();

    let mut state_main = State::new("main".to_string());
    let mut branch = Branch::new();
    branch.add_sym(b'\0');
    branch.add_primitive(Primitive::Movel);
    branch.set_trans(Transition::Continue(1));
    state_main.push(branch);
    let mut branch = Branch::new();
    branch.set_deflt(true);
    branch.add_primitive(Primitive::Mover);
    branch.set_trans(Transition::Continue(0));
    state_main.push(branch);
    machine.push(state_main);

    let mut state_add = State::new("add".to_string());
    let mut branch = Branch::new();
    branch.add_sym(b'1');
    branch.add_primitive(Primitive::Print(b'0'));
    branch.add_primitive(Primitive::Movel);
    branch.set_trans(Transition::Continue(1));
    state_add.push(branch);
    let mut branch = Branch::new();
    branch.set_deflt(true);
    branch.add_primitive(Primitive::Print(b'1'));
    branch.set_trans(Transition::Accept);
    state_add.push(branch);
    machine.push(state_add);

    machine
}

#[derive(Debug, Copy, Clone)]
pub enum ExecutionState {
    Active,
    Accepted,
    Rejected,
    Failed,
}

pub struct Execution {
    machine: Rc<Machine>,
    tape: Tape,
    pos: i32,
    state_idx: usize,
    exec_state: ExecutionState,
}

impl Execution {
    pub fn new(machine: &Rc<Machine>) -> Self {
        Execution {
            machine: Rc::clone(machine),
            tape: Tape::new(),
            pos: 0,
            state_idx: 0,
            exec_state: ExecutionState::Active,
        }
    }

    pub fn from(machine: &Rc<Machine>, s: &str) -> Self {
        Execution {
            machine: Rc::clone(machine),
            tape: Tape::from(s),
            pos: 0,
            state_idx: 0,
            exec_state: ExecutionState::Active,
        }
    }

    pub fn step(&mut self) {
        if let ExecutionState::Active = self.exec_state {
            let sym = self.tape.grow_and_get(self.pos);
            match (*self.machine).get_by_idx(self.state_idx).unwrap().get_by_sym(sym) {
                Some(branch) => {
                    for primitive in branch.get_primitives().iter() {
                        match primitive {
                            Primitive::Movel => { self.pos -= 1; }
                            Primitive::Mover => { self.pos += 1; }
                            Primitive::Print(new_sym) => { self.tape.put(self.pos, *new_sym); }
                        };
                    }
                    self.tape.grow(self.pos); // update tape to fit
                    match branch.get_trans() {
                        Transition::Continue(new_state_idx) => { self.state_idx = new_state_idx; },
                        Transition::Accept => { self.exec_state = ExecutionState::Accepted },
                        Transition::Reject => { self.exec_state = ExecutionState::Rejected },
                    }
                }
                None => {
                    self.exec_state = ExecutionState::Failed;
                }
            }
        }
    }

    pub fn is_active(&self) -> bool {
        match self.exec_state {
            ExecutionState::Active => true,
            _ => false,
        }
    }

    pub fn get_exec_state(&self) -> ExecutionState {
        self.exec_state
    }

    pub fn get_pos(&self) -> i32 {
        self.pos
    }

    pub fn get_sym(&self) -> u8 {
        self.tape.get(self.pos).unwrap()
    }

    pub fn get_state_name(&self) -> &str {
        (*self.machine).get_by_idx(self.state_idx).unwrap().get_name()
    }

    pub fn display_long(&self) {
        // TODO
        self.tape.display_long();
    }
}
