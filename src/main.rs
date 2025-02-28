/*
fn main_concrete() {
    let input = std::fs::read_to_string("test_machine.tm").unwrap();
    match parse_machine(&input) {
        Ok(ParseData {
            machine,
            state_names: _,
        }) => {
            println!("{:?}", machine);
            let mut exec = Execution {
                machine: &machine,
                state_idx: 0,
                pos: 0,
                tape: Tape::from("1011"),
            };
            while let StepEvent::Continue = exec.step() {
                // nothing.
            }
            println!("{:?}", exec.tape);
        }
        Err(e) => println!("{}", e),
    }
}
*/

mod exec;
mod parse;
mod skeleton;
use chumsky::Parser;
use exec::{Execution, StepEvent};

fn main() {
    let input = std::fs::read_to_string("machines/mapped.tm").unwrap();

    let res = match parse::concrete::machine_parser().parse(input) {
        Ok(r) => r,
        Err(e) => panic!("{:#?}", e),
    };
    let mut ex = Execution::new(&res);

    loop {
        match ex.step() {
            StepEvent::Accept => {
                println!("accept!");
                break;
            }
            StepEvent::Reject => {
                println!("reject!");
                break;
            }
            StepEvent::Continue => {
                println!("continue!");
            }
        }
    }
    println!("{} {} {}", ex.tape_get(0), ex.tape_get(1), ex.tape_get(2));
}
