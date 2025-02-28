main {
	#incr(incr('0')) > #incr2('0') accept
}

map incr {
	['0'] '1'
	['1'] '2'
	['2'] '3'
}

map incr2 {
	[!] incr(incr($))
}
