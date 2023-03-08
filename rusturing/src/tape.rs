use std::fmt;

fn translate_display(c: &u8) -> char {
    match c {
        b' ' ..= b'~' => *c as char,
        _ => '_',
    }
}

fn to_hex(c: u8) -> Option<char> {
    match c {
        0 ..= 9   => Some((b'0' + c) as char),
        10 ..= 15 => Some((b'a' + c - 10) as char),
        _ => None
    }
}

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

    pub fn get(&self, index: i32) -> Option<u8> {
        if index < 0 {
            self.left.get(!index as usize).map(|sym| *sym)
        } else {
            self.right.get(index as usize).map(|sym| *sym)
        }
    }

    pub fn grow_and_get(&mut self, index: i32) -> u8 {
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

    pub fn display_long(&self) {
        println!("{}", self); // short version before long version
        for _ in 0..self.len() {
            print!("-");
        }
        println!();
        for c in self.left.iter().rev()
                .chain(self.right.iter())
                .map(|&c| to_hex((c >> 4) & 0x0f).unwrap()) {
            print!("{}", c);
        }
        println!();
        for c in self.left.iter().rev()
                .chain(self.right.iter())
                .map(|&c| to_hex(c & 0x0f).unwrap()) {
            print!("{}", c);
        }
        println!();
    }
}

impl fmt::Display for Tape {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        for c in self.left.iter().rev()
                .chain(self.right.iter())
                .map(translate_display) {
            write!(f, "{}", c)?;
        }
        Ok(())
    }
}
