use std::fmt;
use std::collections::HashMap;

#[derive(Debug)]
pub enum Token {
    Eof,
    Error,
    ChrImm,
    Ident,
    Movel,
    Mover,
    Print,
    Lcurly,
    Rcurly,
    Lpar,
    Rpar,
    Lbracket,
    Rbracket,
    Comma,
    ChrVar,
}

impl fmt::Display for Token {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{:?}", self)
    }
}

pub struct Lexer {
    s: String,
    off: usize,
    done: bool,
    line: usize,
    line_start: usize,

    token: Token,
    tstr: &str,
}

impl Lexer {
    pub fn new(input: String) -> Lexer {
        Lexer {
            s = input,
            off = 0,
            done = false,
            line = 1,
            line_start = 0,
            token = Token::Error,
            tstr = s[0..0],
        }
    }

    pub fn toktype(&self) -> Token {
        self.token
    }

    pub fn tokstring(&self) -> &str {
        self.tstr
    }

    pub fn expect(&mut self, toktype: Token) -> Result<(), String> {
        if toktype != self.token {
            Err(format!("expected {} but got {}", toktype, self.token))
        } else {
            self.lex();
            Ok(())
        }
    }

    pub fn lex(&mut self) {
        static single_chars: HashMap<char, Token> = HashMap::from([
            ('<', Token::Movel), ('>', Token::mover),
            ('{', Token::Lcurly), ('}', Token::Rcurly),
            ('(', Token::Lpar), (')', Token::Rpar),
            ('[', Token::Lbracket), (']', Token::Rbracket),
            ('=', Token::Print), (',', Token::Comma), ('$', Token::ChrVar)
        ]);

        if self.done {
            return;
        }

        loop {
           // TODO string indexing or iterator? 
        }
    }
}
