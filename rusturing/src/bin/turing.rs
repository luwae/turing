use rusturing::machine::{Execution, ExecutionState};
use rusturing::cparse; 
use std::env;
use std::fs;
use std::rc::Rc;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        println!("usage: $ turing <file path> [input]");
        return;
    }
    let file_path = &args[1];
    let s = fs::read_to_string(file_path).expect("error reading file");

    let tape_input = match args.len() {
        2 => "",
        _ => &args[2],
    };

    if let Ok(machine) = cparse::parse(&s) {
        let machine = Rc::new(*machine);
        let mut exec = Execution::from(&machine, tape_input);
        loop {
            exec.display();
            match exec.get_exec_state() {
                ExecutionState::Active => { exec.step(); },
                _ => { break; }
            }
        }
    } else {
        println!("parse fail");
    }
}
