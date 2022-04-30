class TextAnalysis:
    @staticmethod
    def ishexdigit(c):
        return c.isdigit() or (ord(c) >= ord('a') and ord(c) <= ord('f'))

class Token:
    def __init__(self, tp, s):
        self.tp = tp
        self.s = s

    def __repr__(self):
        return f"Token({self.tp}, {self.s})"

    def __str__(self):
        return repr(self)

class LexIter:
    class NullTerminatedString:
        def __init__(self, s):
            self.s = s
        def __getitem__(self, key):
            if key >= 0 and key < len(self.s):
                return self.s[key]
            return "\0"

    def __init__(self, s):
        self.s = LexIter.NullTerminatedString(s)
        self.pos = 0
        self.done = False

    def __next__(self):
        if (self.done):
            raise StopIteration

        s = self.s
        while s[self.pos].isspace():
            self.pos += 1
        
        # check actual character
        single_chars = {
                '>' : "mover", '<' : "movel", '=' : "print" , 
                '{' : "lcurly", '}' : "rcurly",
                '(' : "lpar", ')' : "rpar",
                '[' : "lbra", ']' : "rbra",
                ',' : "comma",
                '$' : "chr_var"
        }
        c = s[self.pos]
        if c == '\0':
            self.done = True
            return Token("eof", None)
        if c in single_chars:
            self.pos += 1
            return Token(single_chars[c], c)
        if c == '\'':
            c1 = s[self.pos + 1]
            c2 = s[self.pos + 2]
            if c1 == 'x':
                if c2 == '\'':
                    self.pos += 3
                    return Token("chr_imm", "x")
                c3 = s[self.pos + 3]
                c4 = s[self.pos + 4]
                if TextAnalysis.ishexdigit(c2) and TextAnalysis.ishexdigit(c3) and c4 == '\'':
                    self.pos += 5
                    return Token("chr_imm", "x" + c2 + c3)
                self.done = True
                return Token("error", None)
            if ord(c1) >= ord('!') and ord(c1) <= ord('~') and c2 == '\'':
                self.pos += 3
                return Token("chr_imm", c1)
            self.done = True
            return Token("error", None)
        if c.isalpha() or c == '_':
            start = self.pos
            self.pos += 1
            while s[self.pos].isalnum() or s[self.pos] == '_':
                self.pos += 1
            return Token("ident", s.s[start:self.pos])
        self.done = True
        return Token("error", None)

class Lexer:
    class NullTerminatedString:
        def __init__(self, s):
            self.s = s
        def __getitem__(self, key):
            if isinstance(key, slice) or (key >= 0 and key < len(self.s)):
                return self.s[key]
            return "\0"

    def __init__(self, s):
        self.s = Lexer.NullTerminatedString(s)
        self.pos = 0
        self.tok = None
        self.done = False
        self.lex()

    def reset(self):
        self.pos = 0
        self.done = False
        self.lex()

    def _lex(self):
        if self.done:
            return

        s = self.s
        while s[self.pos].isspace():
            self.pos += 1
        
        # check actual character
        single_chars = {
                '>' : "mover", '<' : "movel", '=' : "print" , 
                '{' : "lcurly", '}' : "rcurly",
                '(' : "lpar", ')' : "rpar",
                '[' : "lbra", ']' : "rbra",
                ',' : "comma",
                '$' : "chr_var"
        }
        c = s[self.pos]
        if c == '\0':
            self.done = True
            self.tok = Token("eof", None)
            return
        if c in single_chars:
            self.pos += 1
            self.tok = Token(single_chars[c], c)
            return
        if c == '\'':
            c1 = s[self.pos + 1]
            c2 = s[self.pos + 2]
            if c1 == 'x':
                if c2 == '\'':
                    self.pos += 3
                    self.tok = Token("chr_imm", "x")
                    return
                c3 = s[self.pos + 3]
                c4 = s[self.pos + 4]
                if TextAnalysis.ishexdigit(c2) and TextAnalysis.ishexdigit(c3) and c4 == '\'':
                    self.pos += 5
                    self.tok = Token("chr_imm", "x" + c2 + c3)
                    return
                self.done = True
                self.tok = Token("error", None)
                return
            if ord(c1) >= ord('!') and ord(c1) <= ord('~') and c2 == '\'':
                self.pos += 3
                self.tok = Token("chr_imm", c1)
                return
            self.done = True
            self.tok = Token("error", None)
            return
        if c.isalpha() or c == '_':
            start = self.pos
            self.pos += 1
            while s[self.pos].isalnum() or s[self.pos] == '_':
                self.pos += 1
            self.tok = Token("ident", s.s[start:self.pos])
            return
        self.done = True
        self.tok = Token("error", None)

    def lex(self):
        self._lex()
        print(f"lex: {self.tok}")

if __name__ == "__main__":
    t = """
      fr($a, E) {
        [$a] E
        ['f'] <<='f'<< main
        ['x00', 'x0f'] _Other2
        fr($a, E)
      }
    """
    lx = Lexer(t)
    print(lx.tok)
    while lx.tok.tp not in ("eof", "error"):
        lx.lex()
        print(lx.tok)
