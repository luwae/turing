use rusturing::machine::{Execution, ExecutionState};
use rusturing::parse::parse; 
use std::env;
use std::fs;
use std::rc::Rc;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() != 2 {
        println!("usage: $ <program name> <file path>");
        return;
    }
    let file_path = &args[1];
    let s = fs::read_to_string(file_path).expect("error reading file");

    if let Ok(machine) = parse(&s) {
        println!("Ok");
        let machine = Rc::new(*machine);
        let mut exec = Execution::from(&machine, "10011");
        loop {
            exec.display_long();
            match exec.get_exec_state() {
                ExecutionState::Active => {
                    println!("active in state {} at position {}", exec.get_state_name(), exec.get_pos());
                    exec.step();
                },
                ExecutionState::Accepted => {
                    println!("accepted in state {} at position {}", exec.get_state_name(), exec.get_pos());
                    break;
                },
                ExecutionState::Rejected => {
                    println!("rejected in state {} at position {}", exec.get_state_name(), exec.get_pos());
                    break;
                },
                ExecutionState::Failed => {
                    println!("failed in state {} at position {} with sym {}", exec.get_state_name(), exec.get_pos(), exec.get_sym());
                    break;
                },
            }
        }
    }
}
