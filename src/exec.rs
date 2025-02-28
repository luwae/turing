#[derive(Debug)]
struct Tape {
    left: Vec<u8>,
    right: Vec<u8>,
}

impl Tape {
    fn new() -> Self {
        Self {
            left: Vec::new(),
            right: Vec::new(),
        }
    }

    fn with_content(s: &str) -> Self {
        let mut right = Vec::new();
        right.extend_from_slice(s.as_bytes());
        Self {
            left: Vec::new(),
            right,
        }
    }

    fn get(&self, idx: i32) -> u8 {
        if idx >= 0 {
            let idx = idx as usize;
            self.right.get(idx).copied().unwrap_or(0)
        } else {
            let idx = (!idx) as usize;
            self.left.get(idx).copied().unwrap_or(0)
        }
    }

    fn get_reserve(&mut self, idx: i32) -> u8 {
        if idx >= 0 {
            let idx = idx as usize;
            if idx >= self.right.len() {
                self.right.resize(idx + 1, 0);
            }
            self.right[idx]
        } else {
            let idx = (!idx) as usize;
            if idx >= self.left.len() {
                self.left.resize(idx + 1, 0);
            }
            self.left[idx]
        }
    }

    fn bounds_inclusive(&self) -> (i32, i32) {
        (
            -TryInto::<i32>::try_into(self.left.len()).unwrap(),
            TryInto::<i32>::try_into(self.right.len()).unwrap() - 1,
        )
    }

    fn set(&mut self, idx: i32, val: u8) {
        if idx >= 0 {
            let idx = idx as usize;
            if idx >= self.right.len() {
                self.right.resize(idx + 1, 0);
            }
            self.right[idx] = val;
        } else {
            let idx = (!idx) as usize;
            if idx >= self.left.len() {
                self.left.resize(idx + 1, 0);
            }
            self.left[idx] = val;
        }
    }
}

#[derive(Debug, Clone)]
pub enum Primitive {
    Movel,
    Mover,
    Print(u8),
}

#[derive(Debug, Clone)]
pub enum Continuation {
    Accept,
    Reject,
    State(usize),
}

pub trait Machine {
    fn primitives(&self, state_idx: usize, sym: u8) -> Option<impl Iterator<Item = Primitive>>;
    fn continuation(&self, state_idx: usize, sym: u8) -> Continuation;
    fn state_name(&self, state_idx: usize) -> &str;
}

pub struct Execution<'a, M: Machine> {
    machine: &'a M,
    state_idx: usize,
    pos: i32,
    tape: Tape,
}

#[derive(Debug)]
pub enum StepEvent {
    Continue,
    Accept,
    Reject,
}

impl<'a, M: Machine> Execution<'a, M> {
    pub fn new(machine: &'a M) -> Self {
        Self {
            machine,
            state_idx: 0,
            pos: 0,
            tape: Tape::new(),
        }
    }

    pub fn with_tape_content(machine: &'a M, tape_content: &str) -> Self {
        Self {
            machine,
            state_idx: 0,
            pos: 0,
            tape: Tape::with_content(tape_content),
        }
    }

    pub fn tape_get(&self, idx: i32) -> u8 {
        self.tape.get(idx)
    }

    pub fn step(&mut self) -> StepEvent {
        let scan = self.tape.get(self.pos);

        if let Some(it) = self.machine.primitives(self.state_idx, scan) {
            for primitive in it {
                match primitive {
                    Primitive::Movel => {
                        self.pos -= 1;
                    }
                    Primitive::Mover => {
                        self.pos += 1;
                    }
                    Primitive::Print(sym) => {
                        self.tape.set(self.pos, sym);
                    }
                }
            }
        }

        match self.machine.continuation(self.state_idx, scan) {
            Continuation::Accept => StepEvent::Accept,
            Continuation::Reject => StepEvent::Reject,
            Continuation::State(state_idx) => {
                self.state_idx = state_idx;
                StepEvent::Continue
            }
        }
    }
}

pub trait Present<'a, M: Machine> {
    fn present(&self, ex: &Execution<'a, M>);
}

pub struct AsciiPresent;

impl<'a, M: Machine> Present<'a, M> for AsciiPresent {
    fn present(&self, ex: &Execution<'a, M>) {
        let bounds = {
            let tape_bounds = ex.tape.bounds_inclusive();
            (
                std::cmp::min(tape_bounds.0, ex.pos),
                std::cmp::max(tape_bounds.1, ex.pos),
            )
        };
        let (center_off, len) = (-bounds.0, -bounds.0 + bounds.1 + 1);

        // position and state name
        let real_pos = center_off + ex.pos;
        for _ in 0..real_pos {
            print!(" ");
        }
        print!("V - {}", ex.machine.state_name(ex.state_idx));
        println!();

        // center
        for i in 0..len {
            print!("{}", if i == center_off { '|' } else { ' ' });
        }
        println!();

        // tape
        for i in 0..len {
            let s = match ex.tape.get(i - center_off) {
                printable if (b' '..=b'~').contains(&printable) && printable != b'_' => {
                    printable as char
                }
                0 => '_',
                err => panic!("non-ascii-able u8: {}", err),
            };
            print!("{}", s);
        }
        println!();
    }
}
