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

fn main() {
    let input = std::fs::read_to_string("machines/mapped.tm").unwrap();

    let res = parse::concrete::machine_parser().parse(input);
    println!("{:#?}", res);
}
