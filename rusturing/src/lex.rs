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
    it: Chars,
    done: bool,
    line: usize,

    token: Token,
    tstr: &str,
}

impl Lexer {
    pub fn new(input: String) -> Lexer {
        Lexer {
            s = input,
            it = input.chars(),
            done = false,
            line = 1,
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

    /// Returns the next character and may increment line count
    fn getch(&mut self) -> Option<char> {
        let c = self.it.next();
        if let Some('\n') = c {
            self.line += 1;
        }
        c
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

        let next = loop {
            match self.getch() {
                Some(c) => {
                    if c.is_whitespace() {
                        continue;
                    }
                    Some(c)
                }
                None => None
            }
        }

        if let None = next {
            self.done = true;
            self.token = Token::Eof;
            self.tstr = s[0..0];
        }
    }

    // TODO use byte ascii indexing instead of iterator.
}
