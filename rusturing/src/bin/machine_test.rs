use rusturing::machine::{self, Machine, Execution, ExecutionState};
use std::rc::Rc;

fn main() {
    let machine: Rc<Machine> = Rc::new(machine::add_one());
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
                println!("failed in state {} at position {} with sym {}", exec.get_state_name(), exec.get_pos(), exec.get_sym() as char);
                break;
            },
        }
    }
}
