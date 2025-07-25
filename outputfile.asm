xor		r(5), r(5)
jmp		LC2
LC7:
xor		r(7), r(7)
jmp		LC3
LC7:
xor		r(9), r(9)
jmp		LC4
LC5:
mov		r(2), r(10)
call		printstr
add		r(10), 1
LC2:
cmp		r(10), 2
jle		LC3
mov		r(2), r(7)
call		printstr
add		r(7), 1
LC4:
cmp		r(7), 1
jle		LC5
mov		r(2), r(5)
call		printstr
add		r(5), 1
LC6:
cmp		r(5), 0
jle		LC7
