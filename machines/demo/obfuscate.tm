"main" {
  ['x5f'] accept
  [def] "obfuscate('x58', r(main))"
}

"obfuscate('x58', r(main))" {
  ['x61'-'x7a', 'x41'-'x5a'] ='x58' "r(main)"
  [def] "r(main)"
}

"r(main)" {
  [def] > "main"
}
