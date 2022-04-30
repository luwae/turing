from lex import Token, Lexer

class ParseError(Exception):
    def __init__(self, msg):
        super().__init__(msg)

class StateArg:
    def __init__(self, tp, name):
        self.tp = tp
        self.name = name

    def __str__(self):
        s = f"{self.tp} {self.name}"

    def __repr__(self):
        return str(self)

class CallArg:
    def __init__(self, tp, arg):
        self.tp = tp
        self.arg = arg

    def __str__(self):
        return str(self.arg)

    def __repr__(self):
        return str(self)

class Call:
    def __init__(self, name, args):
        self.name = name
        self.args = args

class Primitive:
    def __init__(self, tp, arg):
        self.tp = tp
        self.arg = arg

    def __str__(self):
        if self.tp in "<>":
            return self.tp
        elif self.tp == "=":
            return "=" + str(self.arg)
        else:
            raise ValueError

    def __repr__(self):
        return str(self)

class Action:
    def __init__(self, primitives, call):
        self.primitives = primitives
        self.call = call

class Branch:
    def __init__(self, chars, action):
        self.chars = chars # also CallArgs
        self.action = action

    def __str__(self):
        s = "["
        for i, char in enumerate(chars):
            s += str(char)
            if i < len(chars - 1):
                s += ", "
        s += "] "
        return s + str(action)

    def __repr__(self):
        return str(self)

class State:
    def __init__(self, name, args, branches, default):
        self.name = name
        self.args = args
        self.branches = branches
        self.default = default

    def __str__(self):
        s = self.name
        if self.args:
            s += "("
            for i, arg in enumerate(self.args):
                s += str(arg)
                if i < len(self.args) - 1:
                    s += ", "
                else:
                    s += ") {\n"
        else:
            s += " {\n"

        for branch in self.branches:
            # TODO

        s += "}"
        return s
        

    def __repr__(self):
        return str(self)

def expect(lx, tp, cont=True):
    if lx.tok.tp != tp:
        raise ParseError(f"expected {tp}, but got {lx.tok.tp}")
    if cont:
        lx.lex()

def parse(s):
    lx = Lexer(s)
    return parseS(lx)

def parseS(lx):
    print("parseS")
    states = []
    while lx.tok.tp != "eof":
        expect(lx, "ident", False)
        statename = lx.tok.s
        lx.lex()
        stateargs = parseStateargs(lx)
        expect(lx, "lcurly")
        branches, default = parseStatebody(lx)
        expect(lx, "rcurly")
        states.append(State(statename, stateargs, branches, default))
    return states

def parseStateargs(lx):
    print("parseStateargs")
    if lx.tok.tp == "lcurly":
        return []

    args = []
    expect(lx, "lpar")
    while True:
        if lx.tok.tp == "ident":
            args.append(StateArg("s", lx.tok.s))
            lx.lex()
        elif lx.tok.tp == "chr_var":
            lx.lex()
            expect(lx, "ident", False)
            args.append(StateArg("v", lx.tok.s))
            lx.lex()
        else:
            raise ParseError("error parsing state args: no state or char")

        if lx.tok.tp == "rpar":
            lx.lex()
            break
        elif lx.tok.tp == "comma":
            lx.lex()
        else:
            raise ParseError("error parsing state args: no comma or closing par")
    
    return args

def parseStatebody(lx):
    print("parseStatebody")
    branches = []
    default = None
    while lx.tok.tp == "lbra":
        lx.lex()
        charargs = parseCharargs(lx)
        expect(lx, "rbra")
        actions = parseActions(lx)
        obj = parseObj(lx)
        branches.append((charargs, actions, obj))
    
    if lx.tok.tp in ("movel", "mover", "print", "ident"):
        actions = parseActions(lx)
        obj = parseObj(lx)
        default = (actions, obj)

    return branches, default

def parseChar(lx):
    print("parseChar")
    if lx.tok.tp == "chr_imm":
        c = ("i", lx.tok.s)
        lx.lex()
        return c
    expect(lx, "chr_var")
    expect(lx, "ident", False)
    c = ("v", lx.tok.s)
    lx.lex()
    return c

def parseCharargs(lx):
    print("parseCharargs")
    chrs = []
    while True:
        chrs.append(parseChar(lx))
        if lx.tok.tp == "comma":
            lx.lex()
            continue
        if lx.tok.tp == "rbra":
            break

    print("return from parseCharargs")
    return chrs

def parseActions(lx):
    print("parseActions")
    actions = []
    while lx.tok.tp in ("movel", "mover", "print"):
        if lx.tok.tp in ("movel", "mover"):
            actions.append(lx.tok.tp)
            lx.lex()
        else:
            lx.lex()
            actions.append(("print", parseChar(lx)))

    print("return from parseActions")
    return actions

def parseObj(lx):
    print("parseObj")
    expect(lx, "ident", False)
    name = lx.tok.s
    lx.lex()
    obj = name, parseObj2(lx)

    print("return from parseObj")
    return obj

def parseObj2(lx):
    print("parseObj2")
    args = []
    if lx.tok.tp in ("lbra", "movel", "mover", "print", "ident", "rpar"):
        return
    expect(lx, "lpar")
    while True:
        if lx.tok.tp in ("chr_imm", "chr_var"):
            args.append(parseChar(lx))
        elif lx.tok.tp == "ident":
            args.append(parseObj(lx))
        if lx.tok.tp == "comma":
            lx.lex()
            continue
        if lx.tok.tp == "rpar":
            lx.lex()
            break

    print("return from parseObj2")
    return args


if __name__ == "__main__":
    t = """
      end { }
      main {
        fr('_', p('X', end))
      }
      fr($a, E) {
        [$a] E
        > fr($a, E)
      }
    """
    states = parse(t)
    print(states)
