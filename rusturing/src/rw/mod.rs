use std::cmp;
use std::rc::Rc;

pub mod machine;

#[derive(Debug, Copy, Clone)]
struct Position {
    off: usize,
    lineno: usize,
    lineoff: usize,
}

impl Position {
    fn new() -> Self {
        Position {
            off: 0,
            lineno: 1,
            lineoff: 0,
        }
    }
}

#[derive(Debug, Clone)]
pub struct TokData {
    pos: Position,
    len: usize,
    s: Rc<String>,
}

impl TokData {
    pub fn display_msg(&self, filename: &str, msg: &str) {
        let into_line = self.pos.off - self.pos.lineoff;
        eprintln!("{}:{}:{}: {}", filename, self.pos.lineno, into_line + 1, msg);
        let mut index = self.pos.lineoff;
        loop {
            if index >= self.s.len() {
                eprintln!();
                break;
            }
            let c = self.s.as_bytes()[index];
            if c == b'\n' {
                eprintln!();
                break;
            }
            eprint!("{}", c as char);
            index += 1;
        }
        if self.len > 0 {
            for _ in 0..into_line {
                eprint!(" ");
            }
            for _ in 0..self.len {
                eprint!("~")
            }
            eprintln!();
        }
    }
    
    pub fn substring(&self) -> &str {
        let from = cmp::min(self.pos.off, (*self.s).len());
        let to = cmp::min(self.pos.off + self.len, (*self.s).len());
        &(*self.s)[from .. to]
    }
}

#[derive(Debug, Clone)]
pub struct Token<T> {
    pub tp: T,
    pub data: TokData,
}
