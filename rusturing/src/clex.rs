use crate::lex::{is_hex, is_ident_start, Token, LexerBase};

#[derive(Debug, Copy, Clone, PartialEq, Eq)]
pub enum ConcreteToken {
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

pub struct ConcreteLexer {
    base: LexerBase,
}

impl ConcreteLexer {
    pub fn from(s: &str) -> Self {
        ConcreteLexer {
            base: LexerBase::from(s),
        }
    }

    fn traverse_string_ident(&mut self) -> Result<(), ()> {
        loop {
            let c = self.base.peek();
            if c == b'\"' {
                return Ok(());
            } else if !(b' ' ..= b'~').contains(&c) {
                self.base.back();
                self.base.error("missing terminating \" character");
                return Err(());
            }
        }
    }

    fn traverse_sym(&mut self) -> Result<(), ()> {
        let c1 = self.base.peek();
        if (b' ' ..= b'~').contains(&c1) {
            let c2 = self.base.peek();
            if (c1 != b'\'' && c2 == b'\'') // do not allow '''
                    || (c1 == b'x'
                        && is_hex(c2)
                        && is_hex(self.base.peek())
                        && self.base.peek() == b'\'') {
                return Ok(())
            }
        }
        // fallthrough if anything is wrong
        self.base.back();
        self.base.error("invalid symbol");
        Err(())
    }

    pub fn lex(&mut self) -> Result<Token<ConcreteToken>, ()> {
        self.base.remove_junk();

        let c = self.base.peek();
        let single_char = match c {
            b'\0' => Some(None),
            b'<'  => Some(Some(ConcreteToken::Movel)),
            b'>'  => Some(Some(ConcreteToken::Mover)),
            b'{'  => Some(Some(ConcreteToken::Lcurly)),
            b'}'  => Some(Some(ConcreteToken::Rcurly)),
            b'['  => Some(Some(ConcreteToken::Lbracket)),
            b']'  => Some(Some(ConcreteToken::Rbracket)),
            b'='  => Some(Some(ConcreteToken::Print)),
            b','  => Some(Some(ConcreteToken::Comma)),
            b'-'  => Some(Some(ConcreteToken::Range)),
            _     => None
        };
        
        if let Some(toktype_or_eof) = single_char {
            return Ok(Token::from(toktype_or_eof, self.base.commit()));
        } else if c == b'\"' {
            self.traverse_string_ident()?;
            return Ok(Token::from(Some(ConcreteToken::Ident), self.base.commit()));
        } else if c == b'\'' {
            self.traverse_sym()?;
            return Ok(Token::from(Some(ConcreteToken::Sym), self.base.commit()));
        } else if is_ident_start(c) {
            self.base.back();
            // TODO probably inefficient
            let keywords: Vec<(&'static str, ConcreteToken)> = vec![
                ("accept", ConcreteToken::Accept),
                ("reject", ConcreteToken::Reject),
                ("def", ConcreteToken::Deflt),
            ];
            let tt = match keywords.iter().find(|(kw, _)| self.base.test_keyword(kw)) {
                Some((_, toktype)) => *toktype,
                None => {
                    self.base.traverse_ident();
                    ConcreteToken::Ident
                }
            };
            return Ok(Token::from(Some(tt), self.base.commit()));
        } else {
            self.base.error("invalid character");
            return Err(());
        }
    }
}
