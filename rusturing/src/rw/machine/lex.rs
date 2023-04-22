use std::rc::Rc;
use crate::rw::{Position, TokData, Token};

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

#[derive(Debug, Copy, Clone, PartialEq, Eq)]
pub enum TmTok {
    Ident,
    Sym,
    Movel,
    Mover,
    Lpar,
    Rpar,
    Lcurly,
    Rcurly,
    Lbracket,
    Rbracket,
    Print,
    Comma,
    Range,
    Excl,
    Accept,
    Reject,
    Deflt,
}

pub enum LexError {
    InvalidChar(u8, TokData),
    MalformedSym(TokData),
}

pub type Result<T> = std::result::Result<T, LexError>;

pub struct Lexer {
    start_pos: Position,
    pos: Position,
    s: Rc<String>,
}

impl Lexer {
    pub fn from(s: &str) -> Self {
        Lexer {
            start_pos: Position::new(),
            pos: Position::new(),
            s: Rc::new(s.to_string()),
        }
    }

    fn get<F: Fn(u8) -> bool>(&mut self, pred: F) -> Option<u8> {
        if self.pos.off >= self.s.len() {
            None
        } else {
            let c = self.s.as_bytes()[self.pos.off];
            if pred(c) {
                self.pos.off += 1;
                Some(c)
            } else {
                None
            }
        }
    }
    
    fn tokdata(&self) -> TokData {
        TokData {
            pos: self.start_pos,
            len: self.pos.off - self.start_pos.off,
            s: Rc::clone(&self.s),
        }
    }
    
    fn done(&self) -> bool {
        self.pos.off >= self.s.len()
    }

    fn walk_ident(&mut self, s: &str, tp: TmTok) -> TmTok {
        let mut is_kw = true;
        for c in s.as_bytes() {
            if let None = self.get(|cc| cc == *c) {
                is_kw = false;
                break;
            }
        }
        while let Some(_) = self.get(|c| is_ident(c)) {
            is_kw = false;
        }
        
        if is_kw {
            tp
        } else {
            TmTok::Ident
        }
    }
    
    fn walk_ident_simple(&mut self) -> TmTok {
        while let Some(_) = self.get(|c| is_ident(c)) {
            // collect
        }
        TmTok::Ident
    }
    
    fn walk_sym(&mut self) -> std::result::Result<TmTok, LexError> {
        match self.get(|c| is_printable(c) && c != b'\'') {
            Some(b'x') => match self.get(|c| is_hex(c) || c == b'\'') {
                Some(b'\'') => Ok(TmTok::Sym),
                _ => match self.get(|c| is_hex(c)) {
                    Some(_) => match self.get(|c| c == b'\'') {
                        Some(_) => Ok(TmTok::Sym),
                        None => Err(LexError::MalformedSym(self.tokdata())),
                    },
                    None => Err(LexError::MalformedSym(self.tokdata())),
                },
            },
            Some(_) => match self.get(|c| c == b'\'') {
                Some(_) => Ok(TmTok::Sym),
                None => Err(LexError::MalformedSym(self.tokdata())),
            },
            None => Err(LexError::MalformedSym(self.tokdata())),
        }
    }

    fn remove_junk(&mut self) {
        while let Some(c) = self.get(|cc| " \n\r\t#".as_bytes().contains(&cc)) {
            if c == b'#' {
                while let Some(_) = self.get(|cc| is_printable(cc)) {
                    // ignore
                }
            } else if c == b'\n' {
                self.pos.lineno += 1;
                self.pos.lineoff = self.pos.off;
            }
        }
    }

    pub fn lex(&mut self) -> Result<Option<Token<TmTok>>> {
        self.remove_junk();
        if self.done() {
            return Ok(None)
        }
        
        self.start_pos = self.pos;
        let res: std::result::Result<TmTok, LexError>
                = match self.get(|c| "<>(){}[]=,-!'".as_bytes().contains(&c) || is_ident_start(c)) {
            Some(b'<') => Ok(TmTok::Movel),
            Some(b'>') => Ok(TmTok::Mover),
            Some(b'(') => Ok(TmTok::Lpar),
            Some(b')') => Ok(TmTok::Rpar),
            Some(b'{') => Ok(TmTok::Lcurly),
            Some(b'}') => Ok(TmTok::Rcurly),
            Some(b'[') => Ok(TmTok::Lbracket),
            Some(b']') => Ok(TmTok::Rbracket),
            Some(b'=') => Ok(TmTok::Print),
            Some(b',') => Ok(TmTok::Comma),
            Some(b'-') => Ok(TmTok::Range),
            Some(b'!') => Ok(TmTok::Excl),
            Some(b'\'') => self.walk_sym(),
            Some(b'a') => Ok(self.walk_ident(&"accept"[1..], TmTok::Accept)),
            Some(b'r') => Ok(self.walk_ident(&"reject"[1..], TmTok::Reject)),
            Some(b'd') => Ok(self.walk_ident(&"def"[1..], TmTok::Deflt)),
            Some(_) => Ok(self.walk_ident_simple()),
            None => Err(LexError::InvalidChar(self.s.as_bytes()[self.pos.off], self.tokdata()))
        };
        
        match res {
            Ok(tp) => Ok(Some(Token { tp, data: self.tokdata() })),
            Err(lxerr) => Err(lxerr)
        }
    }
}
