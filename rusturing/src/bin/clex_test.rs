use std::env;
use std::fs;
use rusturing::clex::ConcreteLexer;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() != 2 {
        println!("usage: $ <program name> <file path>");
        return;
    }
    let file_path = &args[1];
    let s = fs::read_to_string(file_path).expect("error reading file");

    let mut lx = ConcreteLexer::from(&s);
    loop {
        match lx.lex() {
            Ok(Some(tok)) => {
                tok.display_context();
                println!("{:?}", tok.toktype);
            }
            Ok(None) => {
                println!("EOF");
                break;
            }
            Err(()) => {
                println!("ERROR");
                break;
            }
        }
        println!();
    }
}
