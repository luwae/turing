"main" {
  [def] >>='x41'<='x31'<='x24'<='x6e'<='x24'<='x20'<='x23'<='x31'<='x31'<='x30'<='x23'<='x31'<='x30'<='x30'<='x23'<='x25'> "singleterm(accept)"
}

"singleterm(accept)" {
  ['x25'] ='x20' "fl('_', newterm(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))"
  [def] "fl('x25', p('x20', rfr('x20', p('x25', l(step(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))))))"
}

"fl('_', newterm(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))" {
  ['_'] "newterm(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))"
  [def] < 
}

"newterm(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))" {
  [def] ='x23'>> "fr('x20', l(l(newterm_rec(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))))"
}

"fr('x20', l(l(newterm_rec(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))))" {
  ['x20'] "l(l(newterm_rec(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))))"
  [def] > 
}

"l(l(newterm_rec(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))))" {
  [def] < "l(newterm_rec(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))"
}

"l(newterm_rec(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))" {
  [def] < "newterm_rec(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))"
}

"newterm_rec(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))" {
  ['x30'] ='x61' "fl('_', p('x30', fr2('x61', 'x62', resubst(l(newterm_rec(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))))))"
  ['x31'] ='x62' "fl('_', p('x31', fr2('x61', 'x62', resubst(l(newterm_rec(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))))))"
  [def] "fl('_', p('x30', l(p('x23', rfr('x23', l(l(add_bit(fl('x23', r(r(newterm_prune(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))))))))))))"
}

"fl('_', p('x30', fr2('x61', 'x62', resubst(l(newterm_rec(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))))))" {
  ['_'] "p('x30', fr2('x61', 'x62', resubst(l(newterm_rec(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))))))"
  [def] < 
}

"p('x30', fr2('x61', 'x62', resubst(l(newterm_rec(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))))))" {
  [def] ='x30' "fr2('x61', 'x62', resubst(l(newterm_rec(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))))"
}

"fr2('x61', 'x62', resubst(l(newterm_rec(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))))" {
  ['x61', 'x62'] "resubst(l(newterm_rec(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))))"
  [def] > 
}

"resubst(l(newterm_rec(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))))" {
  ['x61', 'x41'] ='x30' "l(newterm_rec(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))"
  ['x62', 'x42'] ='x31' "l(newterm_rec(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))"
}

"fl('_', p('x31', fr2('x61', 'x62', resubst(l(newterm_rec(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))))))" {
  ['_'] "p('x31', fr2('x61', 'x62', resubst(l(newterm_rec(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))))))"
  [def] < 
}

"p('x31', fr2('x61', 'x62', resubst(l(newterm_rec(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))))))" {
  [def] ='x31' "fr2('x61', 'x62', resubst(l(newterm_rec(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))))"
}

"fl('_', p('x30', l(p('x23', rfr('x23', l(l(add_bit(fl('x23', r(r(newterm_prune(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))))))))))))" {
  ['_'] "p('x30', l(p('x23', rfr('x23', l(l(add_bit(fl('x23', r(r(newterm_prune(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))))))))))))"
  [def] < 
}

"p('x30', l(p('x23', rfr('x23', l(l(add_bit(fl('x23', r(r(newterm_prune(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))))))))))))" {
  [def] ='x30' "l(p('x23', rfr('x23', l(l(add_bit(fl('x23', r(r(newterm_prune(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))))))))))"
}

"l(p('x23', rfr('x23', l(l(add_bit(fl('x23', r(r(newterm_prune(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))))))))))" {
  [def] < "p('x23', rfr('x23', l(l(add_bit(fl('x23', r(r(newterm_prune(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))))))))))"
}

"p('x23', rfr('x23', l(l(add_bit(fl('x23', r(r(newterm_prune(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))))))))))" {
  [def] ='x23' "rfr('x23', l(l(add_bit(fl('x23', r(r(newterm_prune(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))))))))"
}

"rfr('x23', l(l(add_bit(fl('x23', r(r(newterm_prune(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))))))))" {
  [def] > "fr('x23', l(l(add_bit(fl('x23', r(r(newterm_prune(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))))))))"
}

"fr('x23', l(l(add_bit(fl('x23', r(r(newterm_prune(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))))))))" {
  ['x23'] "l(l(add_bit(fl('x23', r(r(newterm_prune(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))))))))"
  [def] > 
}

"l(l(add_bit(fl('x23', r(r(newterm_prune(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))))))))" {
  [def] < "l(add_bit(fl('x23', r(r(newterm_prune(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))))))"
}

"l(add_bit(fl('x23', r(r(newterm_prune(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))))))" {
  [def] < "add_bit(fl('x23', r(r(newterm_prune(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))))))"
}

"add_bit(fl('x23', r(r(newterm_prune(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))))))" {
  ['x30'] ='x31' "fl('x23', r(r(newterm_prune(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))))"
  ['x31'] ='x30'< 
}

"fl('x23', r(r(newterm_prune(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))))" {
  ['x23'] "r(r(newterm_prune(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))))"
  [def] < 
}

"r(r(newterm_prune(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))))" {
  [def] > "r(newterm_prune(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))"
}

"r(newterm_prune(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))" {
  [def] > "newterm_prune(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))"
}

"newterm_prune(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))" {
  ['x30'] <='x23'<='_'> "rfr('x23', l(newterm_numerator(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))))"
  [def] "rfr('x23', l(newterm_numerator(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))))"
}

"rfr('x23', l(newterm_numerator(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))))" {
  [def] > "fr('x23', l(newterm_numerator(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))))"
}

"fr('x23', l(newterm_numerator(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))))" {
  ['x23'] "l(newterm_numerator(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))"
  [def] > 
}

"l(newterm_numerator(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))" {
  [def] < "newterm_numerator(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))"
}

"newterm_numerator(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))" {
  ['x30', 'x31'] "subst(fl('_', p('x30', fr2('x61', 'x62', resubst(l(newterm_numerator(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))))))))"
  [def] <='x31' "fl('_', p('x23', l(p('x25', fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))))"
}

"subst(fl('_', p('x30', fr2('x61', 'x62', resubst(l(newterm_numerator(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))))))))" {
  ['x30'] ='x61' "fl('_', p('x30', fr2('x61', 'x62', resubst(l(newterm_numerator(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))))))"
  ['x31'] ='x62' "fl('_', p('x30', fr2('x61', 'x62', resubst(l(newterm_numerator(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))))))"
}

"fl('_', p('x30', fr2('x61', 'x62', resubst(l(newterm_numerator(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))))))" {
  ['_'] "p('x30', fr2('x61', 'x62', resubst(l(newterm_numerator(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))))))"
  [def] < 
}

"p('x30', fr2('x61', 'x62', resubst(l(newterm_numerator(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))))))" {
  [def] ='x30' "fr2('x61', 'x62', resubst(l(newterm_numerator(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))))"
}

"fr2('x61', 'x62', resubst(l(newterm_numerator(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))))" {
  ['x61', 'x62'] "resubst(l(newterm_numerator(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))))"
  [def] > 
}

"resubst(l(newterm_numerator(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))))" {
  ['x61', 'x41'] ='x30' "l(newterm_numerator(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))"
  ['x62', 'x42'] ='x31' "l(newterm_numerator(fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))"
}

"fl('_', p('x23', l(p('x25', fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))))" {
  ['_'] "p('x23', l(p('x25', fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))))"
  [def] < 
}

"p('x23', l(p('x25', fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))))" {
  [def] ='x23' "l(p('x25', fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))"
}

"l(p('x25', fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))))" {
  [def] < "p('x25', fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))"
}

"p('x25', fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))))" {
  [def] ='x25' "fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))"
}

"fr('x24', r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))))" {
  ['x24'] "r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))"
  [def] > 
}

"r(singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))))" {
  [def] > "singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))"
}

"singleterm_changepn(r(r(r(subst(rfr('x61', resubst(singleterm(accept))))))), r(r(r(substbig(rfr('x61', resubst(singleterm(accept))))))))" {
  ['x70'] ='x6e' "r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))"
  ['x6e'] ='x70' "r(r(r(subst(rfr('x61', resubst(singleterm(accept)))))))"
}

"r(r(r(substbig(rfr('x61', resubst(singleterm(accept)))))))" {
  [def] > "r(r(substbig(rfr('x61', resubst(singleterm(accept))))))"
}

"r(r(substbig(rfr('x61', resubst(singleterm(accept))))))" {
  [def] > "r(substbig(rfr('x61', resubst(singleterm(accept)))))"
}

"r(substbig(rfr('x61', resubst(singleterm(accept)))))" {
  [def] > "substbig(rfr('x61', resubst(singleterm(accept))))"
}

"substbig(rfr('x61', resubst(singleterm(accept))))" {
  ['x30'] ='x41' "rfr('x61', resubst(singleterm(accept)))"
  ['x31'] ='x42' "rfr('x61', resubst(singleterm(accept)))"
}

"rfr('x61', resubst(singleterm(accept)))" {
  [def] > "fr('x61', resubst(singleterm(accept)))"
}

"fr('x61', resubst(singleterm(accept)))" {
  ['x61'] "resubst(singleterm(accept))"
  [def] > 
}

"resubst(singleterm(accept))" {
  ['x61', 'x41'] ='x30' "singleterm(accept)"
  ['x62', 'x42'] ='x31' "singleterm(accept)"
}

"r(r(r(subst(rfr('x61', resubst(singleterm(accept)))))))" {
  [def] > "r(r(subst(rfr('x61', resubst(singleterm(accept))))))"
}

"r(r(subst(rfr('x61', resubst(singleterm(accept))))))" {
  [def] > "r(subst(rfr('x61', resubst(singleterm(accept)))))"
}

"r(subst(rfr('x61', resubst(singleterm(accept)))))" {
  [def] > "subst(rfr('x61', resubst(singleterm(accept))))"
}

"subst(rfr('x61', resubst(singleterm(accept))))" {
  ['x30'] ='x61' "rfr('x61', resubst(singleterm(accept)))"
  ['x31'] ='x62' "rfr('x61', resubst(singleterm(accept)))"
}

"fl('x25', p('x20', rfr('x20', p('x25', l(step(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))))))" {
  ['x25'] "p('x20', rfr('x20', p('x25', l(step(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))))))"
  [def] < 
}

"p('x20', rfr('x20', p('x25', l(step(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))))))" {
  [def] ='x20' "rfr('x20', p('x25', l(step(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))))"
}

"rfr('x20', p('x25', l(step(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))))" {
  [def] > "fr('x20', p('x25', l(step(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))))"
}

"fr('x20', p('x25', l(step(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))))" {
  ['x20'] "p('x25', l(step(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))))"
  [def] > 
}

"p('x25', l(step(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))))" {
  [def] ='x25' "l(step(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))"
}

"l(step(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))" {
  [def] < "step(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))"
}

"step(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))" {
  [def] "lfl('x23', shl(rfr('x23', cmpge(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))))"
}

"lfl('x23', shl(rfr('x23', cmpge(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))))" {
  [def] < "fl('x23', shl(rfr('x23', cmpge(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))))"
}

"fl('x23', shl(rfr('x23', cmpge(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))))" {
  ['x23'] "shl(rfr('x23', cmpge(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))))"
  [def] < 
}

"shl(rfr('x23', cmpge(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))))" {
  [def] "lfl('x23', r(r(shl1(rfr('x23', cmpge(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))))))"
}

"lfl('x23', r(r(shl1(rfr('x23', cmpge(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))))))" {
  [def] < "fl('x23', r(r(shl1(rfr('x23', cmpge(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))))))"
}

"fl('x23', r(r(shl1(rfr('x23', cmpge(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))))))" {
  ['x23'] "r(r(shl1(rfr('x23', cmpge(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))))))"
  [def] < 
}

"r(r(shl1(rfr('x23', cmpge(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))))))" {
  [def] > "r(shl1(rfr('x23', cmpge(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))))"
}

"r(shl1(rfr('x23', cmpge(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))))" {
  [def] > "shl1(rfr('x23', cmpge(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))))"
}

"shl1(rfr('x23', cmpge(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))))" {
  ['x30'] <='x30'>> "shl1(rfr('x23', cmpge(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))))"
  ['x31'] <='x31'>> "shl1(rfr('x23', cmpge(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))))"
  [def] <='x30'> "rfr('x23', cmpge(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))"
}

"rfr('x23', cmpge(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))" {
  [def] > "fr('x23', cmpge(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))"
}

"fr('x23', cmpge(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))" {
  ['x23'] "cmpge(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))"
  [def] > 
}

"cmpge(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))" {
  [def] "lfl('x23', r(subst(l(lfl('x23', r(cmpge_entry(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))))))))"
}

"lfl('x23', r(subst(l(lfl('x23', r(cmpge_entry(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))))))))" {
  [def] < "fl('x23', r(subst(l(lfl('x23', r(cmpge_entry(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))))))))"
}

"fl('x23', r(subst(l(lfl('x23', r(cmpge_entry(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))))))))" {
  ['x23'] "r(subst(l(lfl('x23', r(cmpge_entry(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))))))"
  [def] < 
}

"r(subst(l(lfl('x23', r(cmpge_entry(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))))))" {
  [def] > "subst(l(lfl('x23', r(cmpge_entry(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))))))"
}

"subst(l(lfl('x23', r(cmpge_entry(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))))))" {
  ['x30'] ='x61' "l(lfl('x23', r(cmpge_entry(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))))"
  ['x31'] ='x62' "l(lfl('x23', r(cmpge_entry(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))))"
}

"l(lfl('x23', r(cmpge_entry(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))))" {
  [def] < "lfl('x23', r(cmpge_entry(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))))"
}

"lfl('x23', r(cmpge_entry(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))))" {
  [def] < "fl('x23', r(cmpge_entry(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))))"
}

"fl('x23', r(cmpge_entry(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))))" {
  ['x23'] "r(cmpge_entry(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))"
  [def] < 
}

"r(cmpge_entry(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))" {
  [def] > "cmpge_entry(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))"
}

"cmpge_entry(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))" {
  ['x30'] ='x61' "rfr2('x61', 'x62', cmpge_0(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))"
  ['x31'] ='x62' "rfr2('x61', 'x62', cmpge_1(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))"
  [def] "rfr('x23', sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))"
}

"rfr2('x61', 'x62', cmpge_0(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))" {
  [def] > "fr2('x61', 'x62', cmpge_0(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))"
}

"fr2('x61', 'x62', cmpge_0(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))" {
  ['x61', 'x62'] "cmpge_0(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))"
  [def] > 
}

"cmpge_0(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))" {
  ['x61'] ='x30'> "cmpge_redo(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))"
  ['x62'] ='x31' "cmpge_leave(fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))"
}

"cmpge_redo(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))" {
  ['x30'] ='x61'< "fl2('x61', 'x62', resubst(r(cmpge_entry(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))))"
  ['x31'] ='x62'< "fl2('x61', 'x62', resubst(r(cmpge_entry(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))))"
  [def] "fl2('x61', 'x62', resubst(fr('x23', rfr('x23', sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))))))"
}

"fl2('x61', 'x62', resubst(r(cmpge_entry(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))))" {
  ['x61', 'x62'] "resubst(r(cmpge_entry(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))))"
  [def] < 
}

"resubst(r(cmpge_entry(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))))" {
  ['x61', 'x41'] ='x30' "r(cmpge_entry(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))"
  ['x62', 'x42'] ='x31' "r(cmpge_entry(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))"
}

"fl2('x61', 'x62', resubst(fr('x23', rfr('x23', sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))))))" {
  ['x61', 'x62'] "resubst(fr('x23', rfr('x23', sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))))"
  [def] < 
}

"resubst(fr('x23', rfr('x23', sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))))" {
  ['x61', 'x41'] ='x30' "fr('x23', rfr('x23', sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))))"
  ['x62', 'x42'] ='x31' "fr('x23', rfr('x23', sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))))"
}

"fr('x23', rfr('x23', sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))))" {
  ['x23'] "rfr('x23', sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))"
  [def] > 
}

"rfr('x23', sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))" {
  [def] > "fr('x23', sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))"
}

"fr('x23', sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))" {
  ['x23'] "sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))"
  [def] > 
}

"sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))" {
  [def] "lfl('x23', l(subst(r(rfr('x23', l(sub_entry(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))))))))"
}

"lfl('x23', l(subst(r(rfr('x23', l(sub_entry(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))))))))" {
  [def] < "fl('x23', l(subst(r(rfr('x23', l(sub_entry(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))))))))"
}

"fl('x23', l(subst(r(rfr('x23', l(sub_entry(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))))))))" {
  ['x23'] "l(subst(r(rfr('x23', l(sub_entry(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))))))"
  [def] < 
}

"l(subst(r(rfr('x23', l(sub_entry(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))))))" {
  [def] < "subst(r(rfr('x23', l(sub_entry(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))))))"
}

"subst(r(rfr('x23', l(sub_entry(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))))))" {
  ['x30'] ='x61' "r(rfr('x23', l(sub_entry(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))))"
  ['x31'] ='x62' "r(rfr('x23', l(sub_entry(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))))"
}

"r(rfr('x23', l(sub_entry(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))))" {
  [def] > "rfr('x23', l(sub_entry(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))))"
}

"rfr('x23', l(sub_entry(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))))" {
  [def] > "fr('x23', l(sub_entry(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))))"
}

"fr('x23', l(sub_entry(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))))" {
  ['x23'] "l(sub_entry(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))"
  [def] > 
}

"l(sub_entry(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))" {
  [def] < "sub_entry(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))"
}

"sub_entry(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))" {
  ['x30'] ='x61' "lfl2('x61', 'x62', sub_0(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))"
  ['x31'] ='x62' "lfl2('x61', 'x62', sub_1(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))"
}

"lfl2('x61', 'x62', sub_0(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))" {
  [def] < "fl2('x61', 'x62', sub_0(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))"
}

"fl2('x61', 'x62', sub_0(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))" {
  ['x61', 'x62'] "sub_0(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))"
  [def] < 
}

"sub_0(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))" {
  [def] "resubst(l(sub_0_1(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))))"
}

"resubst(l(sub_0_1(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))))" {
  ['x61', 'x41'] ='x30' "l(sub_0_1(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))"
  ['x62', 'x42'] ='x31' "l(sub_0_1(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))"
}

"l(sub_0_1(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))" {
  [def] < "sub_0_1(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))"
}

"sub_0_1(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))" {
  ['x30', 'x31'] "subst(sub_redo(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))"
  [def] "fr2('x61', 'x62', resubst(fr('x23', fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))))"
}

"subst(sub_redo(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))" {
  ['x30'] ='x61' "sub_redo(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))"
  ['x31'] ='x62' "sub_redo(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))"
}

"sub_redo(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))" {
  [def] "rfr2('x61', 'x62', resubst(l(sub_entry(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))))"
}

"rfr2('x61', 'x62', resubst(l(sub_entry(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))))" {
  [def] > "fr2('x61', 'x62', resubst(l(sub_entry(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))))"
}

"fr2('x61', 'x62', resubst(l(sub_entry(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))))" {
  ['x61', 'x62'] "resubst(l(sub_entry(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))))"
  [def] > 
}

"resubst(l(sub_entry(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))))" {
  ['x61', 'x41'] ='x30' "l(sub_entry(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))"
  ['x62', 'x42'] ='x31' "l(sub_entry(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))"
}

"fr2('x61', 'x62', resubst(fr('x23', fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))))" {
  ['x61', 'x62'] "resubst(fr('x23', fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))"
  [def] > 
}

"resubst(fr('x23', fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))" {
  ['x61', 'x41'] ='x30' "fr('x23', fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))"
  ['x62', 'x42'] ='x31' "fr('x23', fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))"
}

"fr('x23', fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))" {
  ['x23'] "fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))"
  [def] > 
}

"fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))" {
  ['x61', 'x62', 'x41', 'x42'] "repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))"
  [def] > 
}

"repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))" {
  ['x61', 'x62'] > "repl_frompos(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))"
  ['x41', 'x42'] > "repl_fromneg(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))"
}

"repl_frompos(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))" {
  ['x30', '_'] ='x41'< "calc1(fl('x24', lfl('x24', l(singleterm(accept)))))"
  ['x31'] ='x42'< "calc1(fl('x24', lfl('x24', l(singleterm(accept)))))"
}

"calc1(fl('x24', lfl('x24', l(singleterm(accept)))))" {
  ['x41'] ='x31'< "calc1_decr(fl('x24', lfl('x24', l(singleterm(accept)))))"
  ['x42'] ='x30' "fl('x24', lfl('x24', l(singleterm(accept))))"
  ['x61'] ='x31' "fl('x24', lfl('x24', l(singleterm(accept))))"
  ['x62'] ='x30'< "calc1_incr(fl('x24', lfl('x24', l(singleterm(accept)))))"
}

"calc1_decr(fl('x24', lfl('x24', l(singleterm(accept)))))" {
  ['x30'] ='x31'< "calc1_decr(fl('x24', lfl('x24', l(singleterm(accept)))))"
  ['x31'] ='x30' "fl('x24', lfl('x24', l(singleterm(accept))))"
}

"fl('x24', lfl('x24', l(singleterm(accept))))" {
  ['x24'] "lfl('x24', l(singleterm(accept)))"
  [def] < 
}

"lfl('x24', l(singleterm(accept)))" {
  [def] < "fl('x24', l(singleterm(accept)))"
}

"fl('x24', l(singleterm(accept)))" {
  ['x24'] "l(singleterm(accept))"
  [def] < 
}

"l(singleterm(accept))" {
  [def] < "singleterm(accept)"
}

"calc1_incr(fl('x24', lfl('x24', l(singleterm(accept)))))" {
  ['x30'] ='x31' "fl('x24', lfl('x24', l(singleterm(accept))))"
  ['x31'] ='x30'< "calc1_incr(fl('x24', lfl('x24', l(singleterm(accept)))))"
}

"repl_fromneg(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))" {
  ['x30', '_'] ='x61'< "calc1(fl('x24', lfl('x24', l(singleterm(accept)))))"
  ['x31'] ='x62'< "calc1(fl('x24', lfl('x24', l(singleterm(accept)))))"
}

"lfl2('x61', 'x62', sub_1(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))" {
  [def] < "fl2('x61', 'x62', sub_1(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))"
}

"fl2('x61', 'x62', sub_1(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))" {
  ['x61', 'x62'] "sub_1(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))"
  [def] < 
}

"sub_1(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))" {
  ['x61'] ='x31'< "sub_1_ppg1(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))"
  ['x62'] ='x30'< "sub1_1(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))"
}

"sub_1_ppg1(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))" {
  ['x30'] ='x62'< "sub_1_ppg2(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))"
  ['x31'] ='x61' "sub_redo(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))"
  [def] "rfr2('x61', 'x62', resubst(fr('x23', fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))))"
}

"sub_1_ppg2(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))" {
  ['x30'] ='x31'< "sub_1_ppg2(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))"
  ['x31'] ='x30' "fr2('x61', 'x62', sub_redo(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))"
  [def] "rfr2('x61', 'x62', sub_redo(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))"
}

"fr2('x61', 'x62', sub_redo(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))" {
  ['x61', 'x62'] "sub_redo(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))"
  [def] > 
}

"rfr2('x61', 'x62', sub_redo(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))" {
  [def] > "fr2('x61', 'x62', sub_redo(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))"
}

"rfr2('x61', 'x62', resubst(fr('x23', fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))))" {
  [def] > "fr2('x61', 'x62', resubst(fr('x23', fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))))"
}

"sub1_1(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))" {
  ['x30', 'x31'] "subst(sub_redo(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))"
  [def] "rfr2('x61', 'x62', resubst(fr('x23', fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))))"
}

"cmpge_leave(fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))" {
  [def] "fl2('x61', 'x62', resubst(fr('x23', rfr('x23', fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))))"
}

"fl2('x61', 'x62', resubst(fr('x23', rfr('x23', fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))))" {
  ['x61', 'x62'] "resubst(fr('x23', rfr('x23', fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))))"
  [def] < 
}

"resubst(fr('x23', rfr('x23', fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))))" {
  ['x61', 'x41'] ='x30' "fr('x23', rfr('x23', fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))"
  ['x62', 'x42'] ='x31' "fr('x23', rfr('x23', fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))"
}

"fr('x23', rfr('x23', fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))" {
  ['x23'] "rfr('x23', fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))"
  [def] > 
}

"rfr('x23', fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))" {
  [def] > "fr('x23', fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))"
}

"fr('x23', fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))" {
  ['x23'] "fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))"
  [def] > 
}

"fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))" {
  ['x61', 'x62', 'x41', 'x42'] "repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))"
  [def] > 
}

"repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))" {
  ['x61', 'x62'] > "repl_frompos(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))"
  ['x41', 'x42'] > "repl_fromneg(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))"
}

"repl_frompos(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))" {
  ['x30', '_'] ='x41'< "resubst(fl('x24', lfl('x24', l(singleterm(accept)))))"
  ['x31'] ='x42'< "resubst(fl('x24', lfl('x24', l(singleterm(accept)))))"
}

"resubst(fl('x24', lfl('x24', l(singleterm(accept)))))" {
  ['x61', 'x41'] ='x30' "fl('x24', lfl('x24', l(singleterm(accept))))"
  ['x62', 'x42'] ='x31' "fl('x24', lfl('x24', l(singleterm(accept))))"
}

"repl_fromneg(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))" {
  ['x30', '_'] ='x61'< "resubst(fl('x24', lfl('x24', l(singleterm(accept)))))"
  ['x31'] ='x62'< "resubst(fl('x24', lfl('x24', l(singleterm(accept)))))"
}

"rfr2('x61', 'x62', cmpge_1(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))" {
  [def] > "fr2('x61', 'x62', cmpge_1(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))"
}

"fr2('x61', 'x62', cmpge_1(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept)))))))))" {
  ['x61', 'x62'] "cmpge_1(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))"
  [def] > 
}

"cmpge_1(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))" {
  ['x61'] ='x30' "cmpge_leave(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))"
  ['x62'] ='x31'> "cmpge_redo(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))), fr4('x61', 'x62', 'x41', 'x42', repl(resubst(fl('x24', lfl('x24', l(singleterm(accept))))))))"
}

"cmpge_leave(sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept)))))))))" {
  [def] "fl2('x61', 'x62', resubst(fr('x23', rfr('x23', sub(fr4('x61', 'x62', 'x41', 'x42', repl(calc1(fl('x24', lfl('x24', l(singleterm(accept))))))))))))"
}
