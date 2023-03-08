use std::env;
use std::fs;
use rusturing::lex;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() != 2 {
        println!("usage: $ <program name> <file path>");
        return;
    }
    let file_path = &args[1];
    let s = fs::read_to_string(file_path).expect("error reading file");

    let mut lx = lex::Lexer::from(&s);
    while !lx.is_done() {
        println!("{:?}", lx.tok_ref());
        lx.tok_ref().perror();
        lx.lex();
    }
    println!("{:?}", lx.tok_ref());
}
