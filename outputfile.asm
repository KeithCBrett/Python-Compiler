xor		r(5), r(5)
mov		r(6), 5
xor		r(7), r(7)
jmp		LC2
LC3:
mov		r(2), r(7)
call		printstr
xor		r(8), r(8)
mov		r(9), 10
xor		r(10), r(10)
jmp		LC4
LC5:
mov		r(2), r(10)
call		printstr
add		r(18446744073709551615), 1
LC6:
add		r(18446744073709551615), 1
LC6:
