use rusturing::tape::Tape;

fn main() {
    let mut tape = Tape::from("Hello World");
    tape.display_long();
    tape.put(-3, b'A');
    tape.display_long();
    tape.put(20, b'X');
    tape.display_long();
}
