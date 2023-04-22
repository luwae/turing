use std::env;
use std::fs;
use rusturing::rw::machine::lex::{Lexer, LexError};

fn main() {
    let args: Vec<String> = env::args().collect();
    let s = fs::read_to_string(&args[1]).unwrap();
    let mut lx = Lexer::from(&s);

    loop {
        match lx.lex() {
            Ok(Some(tok)) => {
                tok.data.display_msg(&args[1], &format!("{:?}", tok.tp));
            }
            Ok(None) => {
                println!("EOF");
                break;
            }
            Err(e) => match e {
                LexError::InvalidChar(c, data) => {
                    data.display_msg(&args[1], &format!("invalid char: 0x{:x}", c));
                    break;
                }
                LexError::MalformedSym(data) => {
                    data.display_msg(&args[1], "malformed sym");
                    break;
                }
            }
        }
    }
}
