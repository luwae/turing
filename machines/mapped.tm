main {
	#incr(incr('0')) > #incr2('0')
}

map incr {
	['0'] '1'
	['1'] '2'
	['2'] '3'
}

map plus2 {
	[!] incr(incr($))
}
