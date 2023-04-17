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
    last_token: Option<Token<ConcreteToken>>,
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
    let last_token = lx.lex()?;
    let mut ps = Parser {
        lx,
        last_token, 
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
                parse_error(&resolve.definition, "could not find this state");
                return Err(());
            }
        }
    }

    Ok(ps.machine)
}

fn parse_error(tok: &Token<ConcreteToken>, msg: &str) {
    tok.data.display_context();
    println!("{}", msg);
}

impl Parser {
    fn nexttoken(&mut self) -> Result<(), ()> {
        self.last_token = self.lx.lex()?;
        Ok(())
    }
    
    fn have_token(&self, toktype: ConcreteToken) -> bool {
        match &self.last_token {
            Some(tok) => tok.toktype == toktype,
            None => false
        }
    }

    fn parse_error_current(&self, msg: &str) {
        match &self.last_token {
            Some(tok) => { parse_error(tok, msg); }
            None => { println!("EOF: {}", msg); }
        }
    }

    fn expect(&self, toktype: ConcreteToken, msg: &str) -> Result<(), ()> {
        if self.have_token(toktype) {
            Ok(())
        } else {
            self.parse_error_current(msg);
            Err(())
        }
    }
    
    fn expect_eof(&self, msg: &str) -> Result<(), ()> {
        if let None = self.last_token {
            Ok(())
        } else {
            self.parse_error_current(msg);
            Err(())
        }
    }

    fn expect_and_nexttoken(&mut self, toktype: ConcreteToken, msg: &str) -> Result<(), ()> {
        self.expect(toktype, msg)?;
        self.nexttoken()
    }

    fn parse_start(&mut self) -> Result<(), ()> {
        self.parse_state()?;
        while self.have_token(ConcreteToken::Ident) {
            self.parse_state()?;
        }
        self.expect_eof("expected state definition or EOF")
    }

    fn parse_state(&mut self) -> Result<(), ()> {
        self.expect(ConcreteToken::Ident, "expected state name")?;
        let name = real_ident(self.last_token.as_ref().unwrap().substring());
        if let Some(state_def) = self.state_defs.get(name) {
            self.parse_error_current("state defined twice");
            parse_error(&state_def.definition, "note: previous definition here");
            return Err(())
        }
        self.state_defs.insert(name.to_string(),
            StateDefinition {
                idx: self.machine.len(), 
                definition: self.last_token.as_ref().unwrap().clone(),   
            },
        );
        let mut state = State::new(name.to_string());
        self.nexttoken()?;
        self.expect_and_nexttoken(ConcreteToken::Lcurly, "expected start of state body")?;
        self.parse_statebody(&mut state)?;
        self.expect_and_nexttoken(ConcreteToken::Rcurly, "expected end of state body")?;
        self.machine.push(state);
        Ok(())
    }

    fn parse_statebody(&mut self, state: &mut State) -> Result<(), ()> {
        while self.have_token(ConcreteToken::Lbracket) {
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
        self.expect_and_nexttoken(ConcreteToken::Lbracket, "expected start of branch selector")?;
        let was_deflt = match self.last_token.as_ref().unwrap().toktype {
            ConcreteToken::Deflt => {
                branch.set_deflt(true);
                self.nexttoken()?;
                true
            },
            _ => {
                self.parse_symbol_or_symrange(branch)?;
                while self.have_token(ConcreteToken::Comma) {
                    self.nexttoken()?;
                    self.parse_symbol_or_symrange(branch)?;
                }
                false
            },
        };
        self.expect_and_nexttoken(ConcreteToken::Rbracket, "expected end of branch selector")?;

        loop {
            if self.have_token(ConcreteToken::Movel) {
                branch.add_primitive(Primitive::Movel);
                self.nexttoken()?;
            } else if self.have_token(ConcreteToken::Mover) {
                branch.add_primitive(Primitive::Mover);
                self.nexttoken()?;
            } else if self.have_token(ConcreteToken::Print) {
                self.nexttoken()?;
                self.expect(ConcreteToken::Sym, "expected symbol to print")?;
                let sym = real_sym(self.last_token.as_ref().unwrap().substring());
                branch.add_primitive(Primitive::Print(sym));
                self.nexttoken()?;
            } else {
                break;
            }
        }

        if self.have_token(ConcreteToken::Accept) {
            branch.set_trans(Transition::Accept);
            self.nexttoken()?;
        } else if self.have_token(ConcreteToken::Reject) {
            branch.set_trans(Transition::Reject);
            self.nexttoken()?;
        } else if self.have_token(ConcreteToken::Ident) {
            self.resolves.push(NameResolve {
                state_idx: self.machine.len(),
                branch_idx: branch_idx,
                definition: self.last_token.as_ref().unwrap().clone(),
            });
            self.nexttoken()?;
        } else {
            branch.set_trans(Transition::Continue(self.machine.len()));
        }
        Ok(was_deflt)
    }
    
    fn parse_symbol_or_symrange(&mut self, branch: &mut Branch) -> Result<(), ()> {
        self.expect(ConcreteToken::Sym, "expected symbol")?;
        let start = real_sym(self.last_token.as_ref().unwrap().substring());
        self.nexttoken()?;
        if self.have_token(ConcreteToken::Range) {
            self.nexttoken()?;
            self.expect(ConcreteToken::Sym, "expected symbol range end")?;
            let end = real_sym(self.last_token.as_ref().unwrap().substring());
            self.nexttoken()?;
            for c in start ..= end { // might be empty, but we explicitly allow that
                branch.add_sym(c);
            }
        } else {
            branch.add_sym(start);
        }
        Ok(())
    }
}
