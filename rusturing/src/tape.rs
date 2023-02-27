use std::fmt;

pub struct Tape {
    left: Vec<u8>,
    right: Vec<u8>,
}

impl Tape {
    pub fn new() -> Self {
        Tape {
            left: Vec::new(),
            right: Vec::new(),
        }
    }

    pub fn from(s: &str) -> Self {
        Tape {
            left: Vec::new(),
            right: s.as_bytes().to_vec(),
        }
    }

    pub fn len_left(&self) -> usize {
        self.left.len()
    }

    pub fn len_right(&self) -> usize {
        self.right.len()
    }

    pub fn len(&self) -> usize {
        self.left.len() + self.right.len()
    }

    pub fn grow(&mut self, index: i32) {
        if index < 0 {
            let index: usize = !index as usize;
            if index >= self.left.len() {
                self.left.resize(index + 1, b'\0');
            }
        } else {
            let index: usize = index as usize;
            if index >= self.right.len() {
                self.right.resize(index + 1, b'\0');
            }
        }
    }

    pub fn get(&mut self, index: i32) -> u8 {
        self.grow(index);
        if index < 0 {
            self.left[!index as usize]
        } else {
            self.right[index as usize]
        }
    }

    pub fn put(&mut self, index: i32, value: u8) {
        self.grow(index);
        if index < 0 {
            self.left[!index as usize] = value;
        } else {
            self.right[index as usize] = value;
        }
    }
}

impl fmt::Display for Tape {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        for c in self.left.rev() {
            match c {
                b'\0' => { write!(f, "_")?; }
                _     => { write!(f, "{}", c)?; }
            }
        }
        for c in self.right() {
            match c {
                b'\0' => { write!(f, "_")?; }
                _     => { write!(f, "{}", c)?; }
            }
        }
        Ok(())
    }
}
