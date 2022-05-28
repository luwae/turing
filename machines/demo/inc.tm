main {
  > fr('x23',inc(end))
}

end { }

fr('x23',inc(end)) {
  ['x23'] inc(end)
  > fr('x23',inc(end))
}

inc(end) {
    < if2('x30',p('x31',fr('x23',end)),'x31',p('x30',inc(end)),rfr('x23',end))
}

if2('x30',p('x31',fr('x23',end)),'x31',p('x30',inc(end)),rfr('x23',end)) {
    ['x30'] p('x31',fr('x23',end))
    ['x31'] p('x30',inc(end))
    rfr('x23',end)
}

p('x31',fr('x23',end)) {
    ='x31' fr('x23',end) 
}

fr('x23',end) {
    ['x23'] end
    > fr('x23',end)
}

p('x30',inc(end)) {
    ='x30' inc(end)
}

rfr('x23',end) {
    > fr('x23',end)
}
