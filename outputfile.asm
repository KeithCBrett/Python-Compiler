xor		r(5), r(5)
jmp		LC2
LC5:
mov		r(2), r(5)
call		printstr
xor		r(7), r(7)
jmp		LC3
LC4:
mov		r(2), r(7)
call		printstr
add		r(7), 1
LC2:
cmp		r(7), 9
jle		LC3
add		r(5), 1
LC4:
cmp		r(5), 4
jle		LC5
