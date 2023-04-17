use std::env;
use std::fs;
use rusturing::cparse;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() != 2 {
        println!("usage: $ <program name> <file path>");
        return;
    }
    let file_path = &args[1];
    let s = fs::read_to_string(file_path).expect("error reading file");

    let _machine = cparse::parse(&s).unwrap();
}
