use crate::lex::{self, Token, LexerBase};

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
            if let Some(c) = self.base.peek() {
                if c == b'\"' {
                    return Ok(());
                } else if !lex::is_printable(c) {
                    self.base.error_last("missing terminating \" character");
                    return Err(());
                }
            }
        }
    }

    fn traverse_sym(&mut self) -> Result<(), ()> {
        if let Some(c1) = self.base.peek() {
            if lex::is_printable(c1) && c1 != b'\'' {
                if let Some(c2) = self.base.peek() {
                    if c2 == b'\'' {
                        return Ok(())
                    } else if c1 == b'x' && lex::is_hex(c2) {
                        if let Some(c3) = self.base.peek() {
                            if lex::is_hex(c3) {
                                if let Some(c4) = self.base.peek() {
                                    if c4 == b'\'' {
                                        return Ok(())
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // fallthrough if anything is wrong
        self.base.error_last("invalid symbol");
        Err(())
    }

    pub fn lex(&mut self) -> Result<Option<Token<ConcreteToken>>, ()> {
        self.base.remove_junk();
        
        if let Some(c) = self.base.peek() {
            let single_char = match c {
                b'<'  => Some(ConcreteToken::Movel),
                b'>'  => Some(ConcreteToken::Mover),
                b'{'  => Some(ConcreteToken::Lcurly),
                b'}'  => Some(ConcreteToken::Rcurly),
                b'['  => Some(ConcreteToken::Lbracket),
                b']'  => Some(ConcreteToken::Rbracket),
                b'='  => Some(ConcreteToken::Print),
                b','  => Some(ConcreteToken::Comma),
                b'-'  => Some(ConcreteToken::Range),
                _     => None
            };
            if let Some(toktype) = single_char {
                return Ok(Some(self.base.token_and_commit(toktype)));
            } else if c == b'\"' {
                self.traverse_string_ident()?;
                return Ok(Some(self.base.token_and_commit(ConcreteToken::Ident)));
            } else if c == b'\'' {
                self.traverse_sym()?;
                return Ok(Some(self.base.token_and_commit(ConcreteToken::Sym)));
            } else if lex::is_ident_start(c) {
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
                return Ok(Some(self.base.token_and_commit(tt)));
            } else {
                self.base.error("invalid character");
                return Err(());
            }
        } else { // EOF
            self.base.back();
            return Ok(None);
        }
    }
}
