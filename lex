NAME = (a-zA-Z_)(a-zA-Z0-9_)*
STATE = [(a-zA-Z_)(a-zA-Z0-9_)*]
LCURLY = {
RCURLY = }
SYMDEF =  |x(0-9a-f)(0-9a-f) or |.
SYM = $x(0-9a-f)(0-9a-f) or $.
LMOVE = <
RMOVE = >
MATCHDEF = ?.(.)+ <whitespace>
WILDDEF = @
