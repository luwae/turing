## Basic Definition of Turing Machines

A _Turing Machine_ (TM) is a deterministic machine working on an infinite, linear _tape_ made of _squares_. A TM has a current position, at which it can read (_scan_) and write (_print_) to the square. It cannot read/write squares at any other position.

Each square can contain a _symbol_, or none.
We define $S$ to be the set of symbols that can appear on the tape including the "empty symbol", also called _blank_ (denoted with underscore `_`). In the following we will also refer to _blank_ as a symbol, even though it's meant to denote absence.

A TM has an internal state which Turing called an "m-configuration". We call them _states_ for simplicity. A state determines the next behavior of a TM in combination with the symbol read on the current square.
For each symbol in $S$, a state defines an ordered set of _operations_ that the machine executes. There can be arbitrarily many, including none at all. Operations are:
- "move one square to the left" (denoted as `L` here)
- "move one square to the right" (denoted as `R` here)
- "print a symbol on the current square" (denoted as `P<sym>` here, e.g. `P0` or `P1`).

For each symbol in $S$, a state also defines another state it enters after the operations have been carried out. We call this a _state transition_, and the new state the _continuation state_.

We define two special states called `accept` and `reject`. They are not actual states, but rather termination commands. A TM terminates immediately after entering one of these states. Indeed, this is the **only** way for a TM to terminate.
Note here that this is entirely arbitrary; we could also have defined a single `end` state, or more than boolean values. We could also take an infinite loop or a user-designated state to mean that the machine has terminated, relying solely on tape contents to interpret the "result" of the computation. However, it is useful to make machine termination explicit and to have a minimal amount of information separate from tape contents.

The base definition is quite rigid, in order to better reason about the machines.
- A TM has at least 1 state. The _initial state_ (the state the TM starts in) must also be defined.
- States are exhaustive: operations and state transition must be defined for **every** symbol in $S$, in **every** state. Consequently, $S$ must be known at construction time.

## Convenience for Construction
We create some convenience around these conventions to simplify machine construction.
- We often define states not as an unordered set but as an ordered list, which means we can take the first state as the initial state, removing the necessity for explicit declaration.
- We can give states the appearance of being non-exhaustive by substituting a `reject` for every symbol for which operations and state transition are not defined. This also has the nice benefit that a TM can not only work on $S$, but also on every superset of $S$. This allows us to define a TM without worrying about explicitly defining $S$.
- It is often the case that the continuation state is the same as the current state. To optimize for this common case, we say that a state transition is optional; not having a state transition means that the continuation state is the current state. Note that this is not the same behavior as the implicit `reject` substitution above: the `reject` substitution happens when the symbol is not mentioned at all in this state. For the implicit transition, the symbol is mentioned in the state along with its set of operations (which can be empty).

None of these convenience transformations change anything about the underlying model.

### Symbol Grouping
In the base model, operations and state transition is defined separately for each symbol. However, often the operations and state transition is the same for many symbols.

This is why we define branches.
A _branch_ is a _selector_ with a set of operations and a transition state.
A _selector_ is an unordered set of symbols.

A branch is "taken" (its operations executed and the machine transitioned to its continuation state) when its selector contains the symbol that is on the scanned square.

Consider the following machine working on the letters $a, b, c, d$. The machine moves to the right, deleting every $a$ it encounters, leaving other symbols unmodified.
The machine can be defined with a single state $f$:

| state | symbol | operations | continuation state |
|-------|--------|------------|--------------------|
| f     | a      | P_, R      | f                  |
| f     | b      | R          | f                  |
| f     | c      | R          | f                  |
| f     | d      | R          | f                  |
| f     | _      | R          | f                  |

As you can see, this is quite cumbersome. With branches, we can combine the definitions for the symbols other than $a$:

| state | symbols    | operations | continuation state |
|-------|------------|------------|--------------------|
| f     | a          | P_, R      | f                  |
| f     | b, c, d, _ | R          | f                  |

Note that here, all the selectors must be mutually exclusive, otherwise it is unclear which path to take.

We will lift this restriction now by ordering the branches. Reading from top to bottom, we take the first branch whose selector includes the scanned symbol. With this we can write the machine above much more conveniently using the entire set $S$:

| state | symbols | operations | continuation state |
|-------|---------|------------|--------------------|
| f     | a       | P_, R      | f                  |
| f     | S       | R          | f                  |

This convention of having a catch-all branch as the last branch is so common that we have a special name for it: the _default branch_. If a default branch is defined, the `reject` substitution from above is unnecessary.

Sometimes a state only contains a single, default branch. In this case we can also remove the symbol selector entirely. We call these states _pass-through states_.

One may think that the way we wrote this implies that we already have to know $S$ here, which is false: $S$ is only used as a symbolic value in the table. Even if we change the underlying $S$ after having defined this machine it still works, and now the default branch correctly catches a different set of symbols (everything except $a$).
This is usually the preferred way of handling unknown symbols.

Branch ordering is also useful in combination with another optimization: symbol ranges.

Imagine a Turing machine working on $S$ containing decimal digits and some other symbols (we are intentionally vague here).
The TM replaces every $3$ with a $5$ and vice versa, leaving all other digits untouched, and printing $x$ over every non-digit.

We could define this machine as follows using unordered branches with mutually exclusive selectors:

| state | symbols                | operations | continuation state |
|-------|------------------------|------------|--------------------|
| f     | 3                      | P5, R      | f                  |
| f     | 5                      | P3, R      | f                  |
| f     | 0, 1, 2, 4, 6, 7, 8, 9 | R          | f                  |
| f     | (other symbols)        | Px, R      | f                  |

We save a some digital ink by using symbol ranges (using Rust's range syntax for simplicity), still with unordered branches:

| state | symbols         | operations | continuation state |
|-------|-----------------|------------|--------------------|
| f     | 3               | P5, R      | f                  |
| f     | 5               | P3, R      | f                  |
| f     | 0..=2, 4, 6..=9 | R          | f                  |
| f     | (other symbols) | Px, R      | f                  |

The breaks in the decimal digit range are ugly. With ordered selectors we can use the entire range:

| state | symbols | operations | continuation state |
|-------|---------|------------|--------------------|
| f     | 3       | P5, R      | f                  |
| f     | 5       | P3, R      | f                  |
| f     | 0..=9   | R          | f                  |
| f     | S       | Px, R      | f                  |

Note that this presupposes a notion of sequential symbols, which doesn't exist in general. However, we work with bytes/ASCII symbols as $S$, which are sequential.

#### Refining Selectors
By now we can specify selectors as a set of single symbols or ranges, or catch-all $S$. We can extend this by thinking about unions, intersections, and complements.
Instead of defining selectors solely as a union of symbols and ranges (which themselves are unions of symbols), we can define more complex structures.

Consider a branch that should match for a decimal digit, but not a $3$ or $5$. We could construct this like above, by filtering out the special cases $3$ and $5$ into other branches first.
Alternatively, we can say the selector is the intersection of:
- a selector containing all digits, and
- a selector containing everything except $3$ and $5$.

The first one can be written as a range. The second one can be written as the complement of:
- the set containing 3 and 5
- in $S$.

In total, using `&` to mean intersection, `|` to mean union, and `!` to mean "complement in $S$", we have `0..=9 & !(3 | 5)`.

While these expressions can become arbitrarily complex, we can always reduce them to a simple set of symbols if we know $S$.

In the following we also don't use $S$ to denote the default branch, rather we write `!`, which can be read as the "complement of nothing", i.e. the complement of the empty set in $S$, also known as $S$.

Creating an empty selector by specifying nothing is not possible. If you ever want to use an empty selector for whatever reason, use `!!`.

## Skeleton Turing Machines

Given here is the origninal _skeleton table_ definition by Turing, for completeness. This uses our custom terminology.

A _skeleton table_ is a blueprint for an actual Turing machine (called _concrete_ TM).
A _state_ in a skeleton table is a blueprint for infinitely many _concrete states_. A state can have zero or more _parameters_ which can be substituted in the state. Parameters can be of type symbol and state.

Consider the state $p$:

```
p(a, E) {
  #'a' E
}
```

This state prints the symbol $a$ and transitions to the state $E$. $E$ can be arbitrary; it can also be a state with parameters.

Here is a complete example:

```
main {
  p'0' > fr('0', p('1', fl('0', accept)))
}

p(a, E) { #a E }
l(E) { < E }
r(E) { > E }

fr(a, E) {
  [a] E
  [!] > fr(a, E)
}

fl(a, E) {
  [a] E
  [!] <
}
```

There are the three foundational states $p, l, r$, which print, move left, and move right. They can be chained together to form arbitrary operations.

The state name $fr$ stands for "find right", which it does: if the desired symbol $a$ is scanned, the machine transitions to the continuation $E$. Otherwise, it moves right and stays in the current state $fr(a, E)$. We could also use the implicit continuation syntax here, like in $fl$.

The $main$ state performs some operations like printing and moving, and then transitions into a state made up of several of the ones we just defined. It is read like normal: the machine finds a $0$ to the right, prints a $1$, finds a $0$ to the left, and accepts.

The actual turing machine is generated by substitution. We call this _state resolving_. The first step of which shown below:

```
main {
  p'0' > "fr('0', p('1', fl('0', accept)))"
}

"fr('0', p('1', fl('0', accept)))" {
  ['0'] "p('1', fl('0', accept)))"
  [!] > "fr('0', p('1', fl('0', accept)))" {
}
```

The state needed here is transformed into a concrete state by substituting arguments. The names of the concrete states here are arbitrary; we chose to use a string representation to better see what's going on. These states do not have parameters anymore.

Note how the $a$ now carries the $0$ value, and the state $E$ is what comes after the comma.

Now there is another unresolved state, with which we continue until all have been resolved.

The final Turing machine looks like this:

```
main {
  p'0' > "fr('0', p('1', fl('0', accept)))"
}

"fr('0', p('1', fl('0', accept)))" {
  ['0'] "p('1', fl('0', accept)))"
  [!] > "fr('0', p('1', fl('0', accept)))"
}

"p('1', fl('0', accept)))" {
  #'1' "fl('0', accept)"
}

"fl('0', accept)" {
  ['0'] accept
  [!] <
}
```

The concrete state $fr('0', ...)$ appears twice, however it is already resolved when we encounter it the second time.

If the quoted names confuse you, substitute random names instead:

```
main {
  p'0' > new_state_0
}

new_state_0 {
  ['0'] new_state_1
  [!] > new_state_0
}

new_state_1 {
  #'1' new_state_2
}

new_state_2 {
  ['0'] accept
  [!] <
}
```

Sanity checking of arguments is done during substitution. For example, you cannot use a symbol where a state is expected.

Of course, resolving doesn't always terminate. One can trivially create an infinite amount of concrete states:

```
main { f(accept) }
f(E) { f(f(E)) }
```

Resolving would lead to:

```
main { "f(accept)" }
"f(accept)" { "f(f(accept))" }
"f(f(accept))" { "f(f(f(accept)))" }
"f(f(f(accept)))" { "f(f(f(f(accept))))" }
...
```

This is explicitly forbidden: a skeleton Turing machine is only sound if it resolves to a finite number of concrete states.

### Selector Parameters
Instead of only allowing symbols and states as parameters, we also allow selector parameters. This can look like follows:

```
main { fr('0'..='9', accept) }

fr(sel, E) {
  [sel] E
  [!] >
}
```

### Chaining

Writing sequences like `fr('0', l(p('1', accept)))` creates entirely too many parentheses. Instead, we can transform it into a _chain_ like follows:

```
fr('0') ~ l ~ p('1') ~ accept
```

This uses a chain operator which is only used for illustration purposes. This operator is right-associative, and it inserts whatever is to the right of it as a trailing argument into the state left of it. Meaning, all states inside the chain have one less argument present.

We ignore this chaining operator to write chains like follows:

```
fr('0') l p('1') accept
```

We also want to use primitives directly inside chains. For example,

```
fr('0') < #'1' accept
```

will be translated to the chain above.

There is an important difference between chains and states: chains can be underspecified at the tail, which states can't be. For example, the chain `<<` cannot be written as a state, as there is an argument missing: `l(l(?))`.

We still allow chains as arguments because it is useful. For example, we can create a generic find function:

```
main { find('0'..='9', >>>, accept) }

find(sel, move, E) {
  [sel] E
  [!] move find(sel, move, E)
}
```

The $find$ in this example searches every third square on the tape until it finds a digit.

We cannot write something like this without chains. For this we would need an "incomplete chain" operator (`?` in the following):

```
main { find('0'..='9', r(r(r(?))), accept) }

find(sel, move, E) {
  [sel] E
  [!] move(find(sel, move, E))
}
```

We don't currently use an operator like this.

The end of an underspecified chain, if there is nothing following it, is always the current state, which is consistent with the implicit current state rule. TODO force to make explicit?

## Outlook

There are a few features which would be nice to have.

### Current Symbol Operator

Imagine you want to write a symbol duplicator state:

```
dup {
  ['a'] > #'a'
  ['b'] > #'b'
  ['c'] > #'c'
  ...
}
```

We cannot create a single branch out of these three branches because they are different, even though the structure is the same.

This would work if we introduced a "current symbol" operator (`$` in the following):

```
dup {
  [!] > #$
}
```

### Static Map

Imagine you commonly use a substitution, like `o` for `0` and `i` for `1`. If you want to search for the substitution based on what's on tape, you would have to do:

```
s {
  ['0'] fr('o', ...)
  ['1'] fr('i', ...)
}
```

We could simplify this by introducing a mapping feature:

```
subst(a) = match a {
  ['0'] 'o'
  ['1'] 'i'
}

s {
  ['0'|'1'] fr(subst($), ...)
}
```

This also needs the "current symbol" operator from above.

We can do fancy stuff with this like increment a digit:

```
incr(a) = match a {
  ['0'] '1'
  ['1'] '2'
  ...
  ['8'] '9'
  ['9'] '0'
}
```

### Let bindings

Sometimes, expressions are reused. It would be convenient to not be forced to repeat them.

```
let digit = '0'..='9'
let hexdigit = digit | 'a'..='f'

main { fr(hexdigit, fr(hexdigit, accept)) }
```

We could also introduce scoping to allow let inside a state, for example.
