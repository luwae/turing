# turing machine to detect palindromes with letters 'a' and 'b'

main {
  ['a'] ='_' > ra
  ['b'] ='_' > rb
  ['_'] accept
  [def] reject
}

ra {
  ['_'] < ra2
  [def] >
}

ra2 {
  ['a'] ='_' < moveback
  [def] reject
}

rb {
  ['_'] < rb2
  [def] >
}

rb2 {
  ['b'] ='_' < moveback
  [def] reject
}

moveback {
  ['_'] > main
  [def] <
}
