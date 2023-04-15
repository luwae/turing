use std::cmp;
use std::rc::Rc;

pub fn is_ident_start(c: u8) -> bool {
    c == b'_'
        || (c >= b'a' && c <= b'z')
        || (c >= b'A' && c <= b'Z')
}

pub fn is_ident(c: u8) -> bool {
    is_ident_start(c)
        || (c >= b'0' && c <= b'9')
}

pub fn is_hex(c: u8) -> bool {
    (c >= b'0' && c <= b'9')
        || (c >= b'a' && c <= b'f')
        || (c >= b'A' && c <= b'F')
}

#[derive(Copy, Clone)]
struct Position {
    off: usize,
    lineno: usize,
    lineoff: usize,
}

impl Position {
    fn start() -> Self {
        Position { off: 0, lineno: 1, lineoff: 0 }
    }
}

pub struct TokenData {
    pos: Position,
    len: usize,
    s: Rc<String>,
}

impl TokenData {
    pub fn display_context(&self) {
        let into_line = self.pos.off - self.pos.lineoff;
        println!(":{}:{}", self.pos.lineno, into_line + 1);
        let mut index = self.pos.lineoff;
        loop {
            if index >= self.s.len() {
                println!();
                break;
            }
            let c = self.s.as_bytes()[index];
            if c == b'\n' {
                println!();
                break;
            }
            print!("{}", c as char);
            index += 1;
        }
        for _ in 0..into_line {
            print!(" ");
        }
        for _ in 0..self.len {
            print!("~")
        }
        println!();
    }
    
    pub fn substring(&self) -> &str {
        let from = cmp::min(self.pos.off, (*self.s).len());
        let to = cmp::min(self.pos.off + self.len, (*self.s).len());
        &(*self.s)[from .. to]
    }
}

pub struct Token<T> {
    pub toktype: Option<T>,
    pub data: TokenData,
}

impl<T> Token<T> {
    pub fn from(toktype: Option<T>, data: TokenData) -> Self {
        Token {
            toktype,
            data,
        }
    }
}

pub struct LexerBase {
    curr: Position,
    comm: Position,
    last: Option<Position>,
    s: Rc<String>,
}

impl LexerBase {
    pub fn from(s: &str) -> Self {
        LexerBase {
            curr: Position::start(),
            comm: Position::start(),
            last: None,
            s: Rc::new(s.to_string()),
        }
    }
    
    pub fn peek(&mut self) -> u8 {
        self.last = Some(self.curr);
        if self.curr.off >= (*self.s).len() {
            self.curr.off += 1;
            return b'\0';
        }
        let c = (*self.s).as_bytes()[self.curr.off];
        if c == b'\n' {
            self.curr.lineno += 1;
            self.curr.lineoff = self.curr.off + 1;
        }
        self.curr.off += 1;
        c
    }

    pub fn back(&mut self) {
        match self.last {
            Some(pos) => {
                self.curr = pos;
                self.last = None;
            },
            None => panic!("back() called twice on lexer"),
        }
    }

    pub fn revert(&mut self) {
        self.curr = self.comm;
        self.last = None;
    }

    pub fn commit(&mut self) -> TokenData {
        let data = TokenData {
            pos: self.comm,
            len: self.curr.off - self.comm.off,
            s: Rc::clone(&self.s),
        };
        self.comm = self.curr;
        data
    }
    
    pub fn error(&mut self, msg: &str) {
        let data = self.commit();
        data.display_context();
        println!("{}", msg);
    }

    pub fn remove_comment(&mut self) {
        let mut c = self.peek();
        while c != b'\n' {
            if c == b'\0' {
                self.back();
                return;
            }
            c = self.peek();
        }
    }

    pub fn remove_junk(&mut self) {
        loop {
            let c = self.peek();
            if c == b'#' {
                self.remove_comment();
            } else if c == b'\0' || (b'!' ..= b'~').contains(&c) {
                self.back();
                self.comm = self.curr;
                return;
            }
        }
    }
    
    pub fn test_keyword(&mut self, keyword: &str) -> bool {
        for &c in keyword.as_bytes() {
            if self.peek() != c {
                self.revert();
                return false;
            }
        }
        let after = self.peek();
        if is_ident(after) {
            self.revert();
            return false;
        }
        self.back();
        true
    }
    
    pub fn traverse_ident(&mut self) {
        while is_ident(self.peek()) {
            // loop
        }
        self.back();
    }
}
