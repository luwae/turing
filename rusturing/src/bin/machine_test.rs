use rusturing::machine::{self, Machine, Execution, ExecutionState};
use std::rc::Rc;

fn main() {
    let machine: Rc<Machine> = Rc::new(machine::add_one());
    let mut exec = Execution::from(&machine, "10011");

    loop {
        exec.display_long();
        if !exec.is_active() {
            break;
        }
        exec.step();
    }
}
