xor		r(5), r(5)
jmp		LC2
LC3:
mov		r(2), r(5)
call		printstr
add		r(5), 1
LC2:
cmp		r(5), 4
jle		LC3
xor		r(7), r(7)
jmp		LC4
LC5:
mov		r(2), r(7)
call		printstr
add		r(7), 1
LC4:
cmp		r(7), 9
jle		LC5
