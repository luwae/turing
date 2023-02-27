use std::cmp;
use std::env;
use std::fs;
use std::fmt;
use std::rc::Rc;

fn is_ident_start(c: u8) -> bool {
    c == b'_'
        || (c >= b'a' && c <= b'z')
        || (c >= b'A' && c <= b'Z')
}

fn is_ident(c: u8) -> bool {
    is_ident_start(c)
        || (c >= b'0' && c <= b'9')
}

fn is_hex(c: u8) -> bool {
    (c >= b'0' && c <= b'9')
        || (c >= b'a' && c <= b'f')
        || (c >= b'A' && c <= b'F')
}

#[derive(Debug, Copy, Clone)]
struct Position {
    off: usize,
    lineno: usize,
    lineoff: usize,
}

impl Position {
    fn new() -> Self {
        Position { off: 0, lineno: 1, lineoff: 0 }
    }

    fn clear(&mut self) {
        self.off = 0;
        self.lineno = 1;
        self.lineoff = 0;
    }
}

#[derive(Debug, Copy, Clone, PartialEq, Eq)]
enum TokenType {
    Eof,
    Error,
    Ident,
    Sym,
    Movel,
    Mover,
    Lcurly,
    Rcurly,
    Lbracket,
    Rbracket,
    Print,
    Comma,
    Range,
    Accept,
    Reject,
    Deflt,
}

struct Token {
    toktype: TokenType,
    pos: Position,
    len: usize,
    s: Rc<String>,
}

impl Token {
    fn from(s: &Rc<String>) -> Self {
        Token {
            toktype: TokenType::Error,
            pos: Position::new(),
            len: 0,
            s: Rc::clone(s),
        }
    }

    fn clear(&mut self) {
        self.toktype = TokenType::Error;
        self.pos.clear();
        self.len = 0;
    }

    fn clear_at(&mut self, pos: &Position) {
        self.toktype = TokenType::Error;
        self.pos = *pos;
        self.len = 0;
    }

    fn substring(&self) -> &str {
        // avoid slicing outside string range (for example for Eof token)
        let from = cmp::min(self.pos.off, (*self.s).len());
        let to = cmp::min(self.pos.off + self.len, (*self.s).len());
        &(*self.s)[from .. to]
    }
}

impl fmt::Debug for Token {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "Token({:?}, {:?}, {:?}, \"{}\")", self.toktype, self.pos, self.len, self.substring())
    }
}

impl Clone for Token {
    fn clone(&self) -> Self {
        Token {
            toktype: self.toktype,
            pos: self.pos,
            len: self.len,
            s: Rc::clone(&self.s)
        }
    }
}

#[derive(Debug)]
struct Lexer {
    tok: Token,
    curr: Position,
    last_active: bool,
    last: Position,
    comm: Position,
    s: Rc<String>,
}

impl Lexer {
    fn from(s : &str) -> Self {
        let stringcopy = Rc::new(s.to_string());
        let mut lx = Lexer {
            tok: Token::from(&stringcopy),
            curr: Position::new(),
            last_active: false,
            last: Position::new(),
            comm: Position::new(),
            s: Rc::clone(&stringcopy),
        };
        lx.lex_nocheck();
        lx
    }

    fn peek(&mut self) -> u8 {
        self.last_active = true;
        self.last = self.curr;
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

    fn back(&mut self) {
        if self.last_active {
            self.last_active = false;
            self.curr = self.last;
        } else {
            panic!("back() called twice on lexer");
        }
    }

    fn commit(&mut self, toktype: TokenType) {
        self.comm = self.curr;
        self.tok.len = self.comm.off - self.tok.pos.off;
        self.tok.toktype = toktype;
    }
    
    fn commit_last(&mut self, toktype: TokenType) {
        self.back();
        self.commit(toktype);
    }

    fn revert(&mut self) {
        self.curr = self.comm;
        self.last_active = false;
    }

    fn remove_comment(&mut self) {
        let mut c = self.peek();
        while c != b'\n' {
            if c == b'\0' {
                self.back();
                return;
            }
            c = self.peek();
        }
    }

    fn remove_junk(&mut self) {
        loop {
            let c = self.peek();
            if c == b'#' {
                self.remove_comment();
            } else if c == b'\0' || (b'!' ..= b'~').contains(&c) {
                self.commit_last(TokenType::Error);
                return;
            }
        }
    }

    fn handle_keyword(&mut self, keyword: &str, toktype: TokenType) -> bool {
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
        self.commit_last(toktype);
        true
    }

    fn handle_ident(&mut self) {
        while is_ident(self.peek()) {
            // loop
        }
        self.commit_last(TokenType::Ident);
    }

    fn handle_string_ident(&mut self) {
        loop {
            let c = self.peek();
            if c == b'\"' {
                self.commit(TokenType::Ident);
                return;
            } else if !(b' ' ..= b'~').contains(&c) {
                self.commit_last(TokenType::Error);
                return;
            }
        }
    }

    fn handle_sym(&mut self) {
        let c1 = self.peek();
        if (b' ' ..= b'~').contains(&c1) {
            let c2 = self.peek();
            if c2 == b'\''
                    || (c1 == b'x'
                        && is_hex(c2)
                        && is_hex(self.peek())
                        && self.peek() == b'\'') {
                self.commit(TokenType::Sym);
                return;
            }
        }
        // fallthrough if anything is wrong
        self.commit_last(TokenType::Error);
    }

    fn lex_nocheck(&mut self) -> TokenType {
        self.remove_junk();
        self.tok.clear_at(&self.curr);

        let c = self.peek();
        let single_char = match c {
            b'\0' => Some(TokenType::Eof),
            b'<'  => Some(TokenType::Movel),
            b'>'  => Some(TokenType::Mover),
            b'{'  => Some(TokenType::Lcurly),
            b'}'  => Some(TokenType::Rcurly),
            b'['  => Some(TokenType::Lbracket),
            b']'  => Some(TokenType::Rbracket),
            b'='  => Some(TokenType::Print),
            b','  => Some(TokenType::Comma),
            b'-'  => Some(TokenType::Range),
            _ => None
        };
        if let Some(toktype) = single_char {
            self.commit(toktype);
        } else if c == b'\"' {
            self.handle_string_ident();
        } else if c == b'\'' {
            self.handle_sym();
        }else if is_ident_start(c) {
            self.back();
            // TODO probably inefficient
            let keywords: Vec<(&'static str, TokenType)> = vec![
                ("accept", TokenType::Accept),
                ("reject", TokenType::Reject),
                ("def", TokenType::Deflt),
            ];
            if let None = keywords.iter().find(|(kw, toktype)| self.handle_keyword(kw, *toktype)) {
                self.handle_ident();
            }
        } else {
            self.commit(TokenType::Error);
        }
        self.tok.toktype
    }

    fn lex(&mut self) -> TokenType {
        if self.is_done() {
            return self.tok.toktype;
        }
        self.lex_nocheck()
    }

    fn toktype(&self) -> TokenType {
        self.tok.toktype
    }

    fn is_done(&self) -> bool {
        self.tok.toktype == TokenType::Eof || self.tok.toktype == TokenType::Error
    }

    fn tok_clone(&self) -> Token {
        self.tok.clone()
    }
    
    fn tok_ref(&self) -> &Token {
        &self.tok
    }
}

// TODO use option instead of returning '\0'?

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() != 2 {
        println!("usage: $ <program name> <file path>");
        return;
    }
    let file_path = &args[1];
    let s = fs::read_to_string(file_path).expect("error reading file");

    let mut lx = Lexer::from(&s);
    while !lx.is_done() {
        println!("{:?}", lx.tok_ref());
        lx.lex();
    }
    println!("{:?}", lx.tok_ref());
}
