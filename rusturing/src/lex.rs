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

pub fn is_printable(c: u8) -> bool {
    (b' ' ..= b'~').contains(&c)
}

pub fn is_junk(c: u8) -> bool {
    c == b' ' || c == b'\n' || c == b'\t' 
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

#[derive(Clone)]
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

#[derive(Clone)]
pub struct Token<T> {
    pub toktype: T,
    pub data: TokenData,
}

impl<T> Token<T> {
    pub fn from(toktype: T, data: TokenData) -> Self {
        Token {
            toktype,
            data,
        }
    }
    
    pub fn display_context(&self) {
        self.data.display_context();
    }
    
    pub fn substring(&self) -> &str {
        self.data.substring()
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
    
    pub fn peek(&mut self) -> Option<u8> {
        self.last = Some(self.curr);
        if self.curr.off >= (*self.s).len() {
            self.curr.off += 1;
            return None;
        }
        let c = (*self.s).as_bytes()[self.curr.off];
        if c == b'\n' {
            self.curr.lineno += 1;
            self.curr.lineoff = self.curr.off + 1;
        }
        self.curr.off += 1;
        Some(c)
    }

    pub fn back(&mut self) {
        match self.last {
            Some(pos) => {
                self.curr = pos;
                self.last = None;
            },
            None => panic!("back() has no last state"),
        }
    }

    pub fn revert(&mut self) {
        self.curr = self.comm;
        self.last = None;
    }

    pub fn commit(&mut self) {
        self.comm = self.curr;
        self.last = None;
    }
    
    pub fn commit_last(&mut self) {
        self.back();
        self.commit();
    }
    
    pub fn tokdata(&self) -> TokenData {
        TokenData {
            pos: self.comm,
            len: self.curr.off - self.comm.off,
            s: Rc::clone(&self.s),
        }
    }
    
    pub fn error(&self, msg: &str) {
        self.tokdata().display_context();
        println!("{}", msg);
    }
    
    pub fn error_last(&mut self, msg: &str) {
        self.back();
        self.error(msg);
    }

    pub fn remove_comment(&mut self) {
        loop {
            if let Some(c) = self.peek() {
                if c == b'\n' {
                    break;
                }
            } else {
                self.back();
                return;
            }
        }
    }

    pub fn remove_junk(&mut self) {
        loop {
            if let Some(c) = self.peek() {
                if c == b'#' {
                    self.remove_comment();
                } else if !is_junk(c) {
                    self.commit_last();
                    return;
                }
            } else {
                self.commit_last();
                return;
            }
        }
    }
    
    pub fn test_keyword(&mut self, keyword: &str) -> bool {
        for &c in keyword.as_bytes() {
            if let Some(cc) = self.peek() {
                if cc != c {
                    self.revert();
                    return false;
                }
            } else {
                self.revert();
                return false;
            }
        }
        
        if let Some(c) = self.peek() {
            if is_ident(c) {
                self.revert();
                return false;
            }
        }
        self.back();
        true
    }
    
    pub fn traverse_ident(&mut self) {
        while let Some(c) = self.peek() {
            if !is_ident(c) {
                break;
            }
        }
        self.back();
    }

    pub fn token_and_commit<T>(&mut self, toktype: T) -> Token<T> {
        let tok = Token::from(toktype, self.tokdata());
        self.commit();
        tok
    }
    
    pub fn token_and_commit_last<T>(&mut self, toktype: T) -> Token<T> {
        self.back();
        self.token_and_commit(toktype)
    }
}
