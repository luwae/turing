main {
  rfr('x23',l(subst(r(rfr('x23',l(add(end)))))))
}
rfr('x23',l(subst(r(rfr('x23',l(add(end))))))) {
  >fr('x23',l(subst(r(rfr('x23',l(add(end)))))))
}
fr('x23',l(subst(r(rfr('x23',l(add(end))))))) {
  ['x23'] l(subst(r(rfr('x23',l(add(end))))))
  >fr('x23',l(subst(r(rfr('x23',l(add(end)))))))
}
l(subst(r(rfr('x23',l(add(end)))))) {
  <subst(r(rfr('x23',l(add(end)))))
}
subst(r(rfr('x23',l(add(end))))) {
  ['x30'] ='x61'r(rfr('x23',l(add(end))))
  ['x31'] ='x62'r(rfr('x23',l(add(end))))
  r(rfr('x23',l(add(end))))
}
r(rfr('x23',l(add(end)))) {
  >rfr('x23',l(add(end)))
}
rfr('x23',l(add(end))) {
  >fr('x23',l(add(end)))
}
fr('x23',l(add(end))) {
  ['x23'] l(add(end))
  >fr('x23',l(add(end)))
}
l(add(end)) {
  <add(end)
}
add(end) {
  ['x30'] ='x61'lfl2('x61','x62',resubst(l(add2(end))))
  ['x31'] ='x62'lfl2('x61','x62',add1(end))
  end
}
lfl2('x61','x62',resubst(l(add2(end)))) {
  <fl2('x61','x62',resubst(l(add2(end))))
}
fl2('x61','x62',resubst(l(add2(end)))) {
  ['x61','x62'] resubst(l(add2(end)))
  <fl2('x61','x62',resubst(l(add2(end))))
}
resubst(l(add2(end))) {
  ['x61'] ='x30'l(add2(end))
  ['x62'] ='x31'l(add2(end))
  l(add2(end))
}
l(add2(end)) {
  <add2(end)
}
add2(end) {
  ['x23'] fr2('x61','x62',resubst(fl('x23',end)))
  subst(rfr2('x61','x62',resubst(l(add(end)))))
}
fr2('x61','x62',resubst(fl('x23',end))) {
  ['x61','x62'] resubst(fl('x23',end))
  >fr2('x61','x62',resubst(fl('x23',end)))
}
resubst(fl('x23',end)) {
  ['x61'] ='x30'fl('x23',end)
  ['x62'] ='x31'fl('x23',end)
  fl('x23',end)
}
fl('x23',end) {
  ['x23'] end
  <fl('x23',end)
}
end {
}
subst(rfr2('x61','x62',resubst(l(add(end))))) {
  ['x30'] ='x61'rfr2('x61','x62',resubst(l(add(end))))
  ['x31'] ='x62'rfr2('x61','x62',resubst(l(add(end))))
  rfr2('x61','x62',resubst(l(add(end))))
}
rfr2('x61','x62',resubst(l(add(end)))) {
  >fr2('x61','x62',resubst(l(add(end))))
}
fr2('x61','x62',resubst(l(add(end)))) {
  ['x61','x62'] resubst(l(add(end)))
  >fr2('x61','x62',resubst(l(add(end))))
}
resubst(l(add(end))) {
  ['x61'] ='x30'l(add(end))
  ['x62'] ='x31'l(add(end))
  l(add(end))
}
lfl2('x61','x62',add1(end)) {
  <fl2('x61','x62',add1(end))
}
fl2('x61','x62',add1(end)) {
  ['x61','x62'] add1(end)
  <fl2('x61','x62',add1(end))
}
add1(end) {
  ['x61'] ='x31'<add2(end)
  ['x62'] ='x30'<add3(end)
}
add3(end) {
  ['x30'] ='x62'rfr2('x61','x62',resubst(l(add(end))))
  ['x31'] ='x61'<add4(end)
  fr2('x61','x62',resubst(fl('x23',end)))
}
add4(end) {
  ['x30'] ='x31'add5(end)
  ['x31'] ='x30'<add4(end)
  add5(end)
}
add5(end) {
  fr2('x61','x62',rfr2('x61','x62',resubst(l(add(end)))))
}
fr2('x61','x62',rfr2('x61','x62',resubst(l(add(end))))) {
  ['x61','x62'] rfr2('x61','x62',resubst(l(add(end))))
  >fr2('x61','x62',rfr2('x61','x62',resubst(l(add(end)))))
}
