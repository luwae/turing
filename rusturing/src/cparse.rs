use crate::machine::{Primitive, Transition, Branch, State, Machine};
use crate::lex::{Token};
use crate::clex::{ConcreteToken, ConcreteLexer};
use std::collections::HashMap;

struct NameResolve {
    state_idx: usize,
    branch_idx: usize,
    definition: Token<ConcreteToken>,
}

struct StateDefinition {
    idx: usize, // to resolve names
    definition: Token<ConcreteToken>, // to print errors on double definitions
}

struct Parser {
    lx: ConcreteLexer,
    last_token: Token<ConcreteToken>,
    state_defs: HashMap<String, StateDefinition>,
    resolves: Vec<NameResolve>,
    machine: Box<Machine>,
}

fn real_ident(ident: &str) -> &str {
    if ident.as_bytes()[0] == b'\"' {
        &ident[1..ident.len()-1]
    } else {
        ident
    }
}

fn from_hex(c: u8) -> Option<u8> {
    match c {
        b'0' ..= b'9' => Some(c - b'0'),
        b'a' ..= b'f' => Some(c - b'a' + 10),
        b'A' ..= b'F' => Some(c - b'A' + 10),
        _ => None,
    }
}

fn real_sym(sym: &str) -> u8 {
    let sym = sym.as_bytes();
    if sym.len() == 5 {
        (from_hex(sym[2]).unwrap() << 4) | from_hex(sym[3]).unwrap()
    } else if sym[1] == b'_' { // blank symbol '_' is mapped to 0x00
        0
    } else {
        sym[1]
    }
}

pub fn parse(input: &str) -> Result<Box<Machine>, ()> {
    let mut lx = ConcreteLexer::from(input);
    let tok = lx.lex()?;
    let mut ps = Parser {
        lx,
        last_token: tok, 
        state_defs: HashMap::new(),
        resolves: Vec::new(),
        machine: Box::new(Machine::new()),
    };
    ps.parse_start()?;

    // resolve names
    for resolve in ps.resolves.iter() {
        match ps.state_defs.get(real_ident(resolve.definition.substring())) {
            Some(state_def) => {
                ps.machine
                    .get_by_idx_mut(resolve.state_idx).unwrap()
                    .get_by_idx_mut(resolve.branch_idx).unwrap()
                    .set_trans(Transition::Continue(state_def.idx));
            }
            None => {
                token_error(&resolve.definition, "could not find this state");
                return Err(());
            }
        }
    }

    Ok(ps.machine)
}

fn token_error(tok: &Token<ConcreteToken>, msg: &str) {
    tok.data.display_context();
    println!("{}", msg);
}

impl Parser {
    fn lex() -> Result<(), ()> {
        self.last_token = self.lx.lex()?;
        Ok(())
    }

    fn token_error_current(&self, msg: &str) {
        token_error(&self.last_token, msg);
    }

    fn expect(&self, toktype: ConcreteToken, msg: &str) -> Result<(), ()> {
        if let Some(t) = self.last_token.toktype && t == toktype {
            Ok(())
        } else {
            self.token_error_current(msg);
            Err(())
        }
    }
    
    fn expect_eof(&self, msg: &str) -> Result<(), ()> {
        if let None = self.last_token.toktype {
            Ok(())
        } else {
            self.token_error_current(msg);
            Err(())
        }
    }

    fn expect_and_lex(&mut self, toktype: ConcreteToken, msg: &str) -> Result<(), ()> {
        self.expect(toktype, msg)?;
        self.lex()
    }

    fn parse_start(&mut self) -> Result<(), ()> {
        self.parse_state()?;
        while self.last_token.toktype == ConcreteToken::Ident {
            self.parse_state()?;
        }
        self.expect_eof("expected state definition or EOF")
    }

    fn parse_state(&mut self) -> Result<(), ()> {
        self.expect(ConcreteToken::Ident, "expected state name")?;
        let name = real_ident(self.last_token.substring());
        if let Some(state_def) = self.state_defs.get(name) {
            self.token_error_current("state defined twice");
            token_error(&state_def.definition, "note: previous definition here");
            return Err(())
        }
        self.state_defs.insert(name.to_string(),
            StateDefinition {
                idx: self.states.len(), 
                definition: self.last_token,
            },
        );
        let mut state = State::new(name.to_string());
        self.lex();
        self.expect_and_lex(ConcreteToken::Lcurly, "expected start of state body")?;
        self.parse_statebody(&mut state)?;
        self.expect_and_lex(ConcreteToken::Rcurly, "expected end of state body")?;
        self.states.push(state);
        Ok(())
    }

    fn parse_statebody(&mut self, state: &mut State) -> Result<(), ()> {
        while let Some(ConcreteToken::Lbracket) = self.last_token.toktype {
            let mut branch = Branch::new();
            let was_deflt = self.parse_branch(&mut branch, state.len())?;
            state.push(branch);
            if was_deflt { // default branch is last branch
                break;
            }
        }
        Ok(())
    }

    fn parse_branch(&mut self, branch: &mut Branch, branch_idx: usize) -> Result<bool, ()> {
        self.expect_and_lex(ConcreteToken::Lbracket, "expected start of branch selector")?;
        let was_deflt = match self.lx.toktype() {
            ConcreteToken::Deflt => {
                branch.set_deflt(true);
                self.lx.lex();
                true
            },
            _ => {
                self.parse_symbol_or_symrange(branch)?;
                while self.lx.toktype() == TokenType::Comma {
                    self.lx.lex();
                    self.parse_symbol_or_symrange(branch)?;
                }
                false
            },
        };
        self.expect_and_lex(TokenType::Rbracket, "expected end of branch selector")?;

        loop {
            match self.lx.toktype() {
                TokenType::Movel => {
                    branch.add_primitive(Primitive::Movel);
                    self.lx.lex();
                },
                TokenType::Mover => {
                    branch.add_primitive(Primitive::Mover);
                    self.lx.lex();
                },
                TokenType::Print => {
                    self.lx.lex();
                    self.expect(TokenType::Sym, "expected symbol to print")?;
                    let sym = real_sym(self.lx.tok_ref().substring());
                    branch.add_primitive(Primitive::Print(sym));
                    self.lx.lex();
                },
                _ => { break; }
            }
        }

        match self.lx.toktype() {
            TokenType::Accept => {
                branch.set_trans(Transition::Accept);
                self.lx.lex();
            },
            TokenType::Reject => {
                branch.set_trans(Transition::Reject);
                self.lx.lex();
            },
            TokenType::Ident => {
                self.resolves.push(NameResolve {
                    state_idx: self.states.len(),
                    branch_idx: branch_idx,
                    definition: self.lx.tok_clone(),
                });
                self.lx.lex();
            },
            _ => { branch.set_trans(Transition::Continue(self.states.len())); }
        }
        Ok(was_deflt)
    }
    
    fn parse_symbol_or_symrange(&mut self, branch: &mut Branch) -> Result<(), ()> {
        self.expect(TokenType::Sym, "expected symbol")?;
        let start = real_sym(self.lx.tok_ref().substring());
        self.lx.lex();
        if self.lx.toktype() == TokenType::Range {
            self.lx.lex();
            self.expect(TokenType::Sym, "expected range end")?;
            let end = real_sym(self.lx.tok_ref().substring());
            self.lx.lex();
            for c in start ..= end { // might be empty, but we explicitly allow that
                branch.add_sym(c);
            }
        } else {
            branch.add_sym(start);
        }
        Ok(())
    }
}
