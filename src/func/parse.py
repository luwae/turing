from lex import Token, Lexer

class ParseError(Exception):
    def __init__(self, msg):
        super().__init__(msg)

@dataclass(frozen=True)
class StateArg:
    """Argument to a machine state.
    Can be either a variable character ("v") or state ("s").
    """
    tp: str
    name: str

    def pretty(self):
        if self.tp == "v":
            return f"${self.name}"
        return self.name

class Call:
    pass

@dataclass(frozen=True)
class CallArg:
    """Argument to a state call or an in-state branch.
    Can be either a variable character ("v"), immediate ("i"), or call ("c").
    """
    tp: str
    arg = str | Call

    def pretty(self):
        if self.tp == "v":
            return f"${self.arg}"
        if self.tp == "i":
            return f"'{self.arg}'"
        return self.arg.pretty()

    def substitute(self, mapping):
        is self.tp == "v":
            return CallArg("v", ) # TODO

@dataclass(frozen=True)
class Call:
    """Concrete state call.
    self.args is a (possibly empty) list of CallArg.
    """
    name: str
    args: list[CallArg]

    def pretty(self):
        if not self.args:
            return self.name
        s = f"{self.name}("
        for i, arg in enumerate(self.args):
            s += arg.pretty()
            if i < len(self.args) - 1:
                s += ", "
        s += ")"
        return s

    def elemental(self):
        return not self.args

    def substitute(self, mapping):
        if StateArg("s", self.name) in mapping and self.args:
            raise ValueError(f"can not substitute non-elemental state {self.name}")
        return Call(self.name, map(lambda c: c.substitute(mapping), self.args))

@dataclass(frozen=True)
class Primitive:
    """Primitive turing operation.
    Can be either move left ("<"), move right (">"), or print("=").
    If print, self.arg is the character to print (CallArg).
    """
    tp: str
    arg: CallArg

    def pretty(self):
        if self.tp in "<>":
            return self.tp
        return "=" + self.arg.pretty()

    def substitute(self, mapping):
        is self.tp == "=":
            return Primitive("=", mapping.get(self.arg, default=self.arg))

@dataclass(frozen=True)
class Action:
    """A compound action with primitives and a state call.
    self.primitives is a list of Primitive.
    self.call is a Call.
    """
    primitives: list[Primitive]
    call: Call

    def pretty(self):
        if not self.primitives:
            return str(self.call)
        s = "".join(map(lambda p: p.pretty(), self.primitives))
        s += " "
        return s + self.call.pretty()

    def elemental(self):
        return elemental(self.call)

    def substitute(self, mapping):
        return Action(map(lambda c: c.substitute(mapping), self.primitives), self.call.substitute(mapping))

@dataclass(frozen=True)
class Branch:
    """A branch consisting of a list of chars (CallArg) and an action.
    """
    chars: list[CallArg]
    action: Action

    def pretty(self):
        s = "["
        for i, char in enumerate(self.chars):
            s += char.pretty()
            if i < len(self.chars) - 1:
                s += ", "
        s += "] "
        return s + self.action.pretty()

    def elemental(self):
        return elemental(self.action)

    def substitute(self, mapping):
        concrete_chars = map(lambda c: mapping.get(c, default=c))
        return Branch(concrete_chars, self.action.substitute(mapping))

@dataclass(frozen=True)
class State:
    """A complete state consisting of a name, state args, branches, and default action.
    """
    name: str
    args: list[StateArg]
    branches: list[Branch]
    default: Action

    def pretty(self):
        s = self.name
        if self.args:
            s += "("
            for i, arg in enumerate(self.args):
                s += arg.pretty()
                if i < len(self.args) - 1:
                    s += ", "
            s += ") {\n"
        else:
            s += " {\n"

        for branch in self.branches:
            s += f"  {branch}\n"
        if self.default:
            s += f"  {self.default}\n"
        s += "}"
        return s

    def elemental(self):
        return not self.args and all(map(lambda b: b.elemental(), self.branches)) and self.default.elemental()

    def apply_args(self, concrete_args):
        if len(concrete_args) != len(self.args):
            raise ValueError(f"invalid number of arguments for state {self.name}: {len(concrete_args)} (expected {len(self.args)})")
        for sa, ca in zip(self.args, concrete_args):
            if (sa.tp == "v" and ca.tp != "i") or (sa.tp == "s" and ca.tp != "c"):
                raise ValueError(f"invalid argument for state {self.name}: {ca} (expected type {sa.tp})")

        mapping = {sa, ca for sa, ca in zip(self.args, concrete_args)}

        concrete_branches = None
        if self.branches:
            concrete_branches = [b.substitute(mapping) for b in self.branches]
        concrete_default = None
        if self.default:
            concrete_default = self.default.substitute(mapping)
        return State(self.name, None, concrete_branches, concrete_default)



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
        branches.append(Branch(charargs, Action(actions, obj)))
    
    if lx.tok.tp in ("movel", "mover", "print", "ident"):
        actions = parseActions(lx)
        obj = parseObj(lx)
        default = Action(actions, obj)

    return branches, default

def parseChar(lx):
    print("parseChar")
    if lx.tok.tp == "chr_imm":
        c = CallArg("i", lx.tok.s)
        lx.lex()
        return c
    expect(lx, "chr_var")
    expect(lx, "ident", False)
    c = CallArg("v", lx.tok.s)
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
            actions.append(Primitive(lx.tok.s))
            lx.lex()
        else:
            lx.lex()
            actions.append(Primitive("=", parseChar(lx)))

    print("return from parseActions")
    return actions

def parseObj(lx):
    print("parseObj")
    expect(lx, "ident", False)
    name = lx.tok.s
    lx.lex()
    obj = Call(name, parseObj2(lx))

    print("return from parseObj")
    return obj

def parseObj2(lx):
    print("parseObj2")
    args = []
    if lx.tok.tp != "lpar":
        print("return from parseObj2")
        return
    expect(lx, "lpar")
    while True:
        if lx.tok.tp in ("chr_imm", "chr_var"):
            args.append(parseChar(lx))
        elif lx.tok.tp == "ident":
            args.append(CallArg("c", parseObj(lx)))
        if lx.tok.tp == "comma":
            lx.lex()
            continue
        if lx.tok.tp == "rpar":
            lx.lex()
            break

    print("return from parseObj2:", len(args))
    return args




def resolve(states, elemental_set, state_name, concrete_args): # list of CallArg
    state_candidates = list(filter(lambda s: s.name == state_name, states))
    if len(state_candidates) != 1:
        raise ValueError(f"no state or more than one state named {state_name}")
    state = state_candidates[0]

    if len(state.args) != len(concrete_args):
        raise ValueError(f"invalid number of arguments for {state_name}: {len(concrete_args)}")
    z = zip(state.args, concrete_args)
    char_args = {}
    state_args = {}
    for sa, ca in z:
        if sa.tp == "v":
            if ca.tp != "i":
                raise ValueError(f"invalid parameter bound to {sa}: {ca}")
            char_args[sa.name] = ca
        elif sa.tp == "s":
            if ca.tp != "c":
                raise ValueError(f"invalid parameter bound to {sa}: {ca}")
            state_args[sa.name] = ca

    resolved_name = state.name
    if state.args:
        resolved_name += "("
        z = zip(state.args, concrete_args)
        for i, (sa, ca) in enumerate(z):
            resolved_name += str(ca)
            if i < len(state.args) - 1:
                resolved_name += ", "
        resolved_name += ")"
    print(resolved_name)
    elemental_set.add(resolved_name)

    # TODO function needed to apply arguments in State



#first actual state, then arguments (pay attention to recursion)

#resolve(main) -> "main"
#resolve(fr, ['_', p('X', end)]) -> "fr('_', p('X', end))"
#resolve()

if __name__ == "__main__":
    t = """
      end { }
      main {
        fr('_', p('X', end))
      }
      p($a, E) {
        =$a E
      }
      fr($a, E) {
        [$a] E
        > fr($a, E)
      }
    """
    states = parse(t)
    print(states)

    main_candidates = list(filter(lambda s: s.name == "main" and not s.args, states))
    if len(main_candidates) != 1:
        raise ValueError("no main state or more than one")

    main = main_candidates[0]

    elemental_set = set()
    resolve(states, elemental_set, "fr", [CallArg("i", "a"), CallArg("c", Call("end", [CallArg("i", "X")]))])
