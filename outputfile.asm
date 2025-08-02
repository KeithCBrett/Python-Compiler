xor		r(5), r(5)
jmp		LC2
LC15:
xor		r(7), r(7)
jmp		LC3
LC19:
xor		r(9), r(9)
jmp		LC4
LC21:
xor		r(11), r(11)
jmp		LC5
LC21:
xor		r(13), r(13)
jmp		LC6
LC19:
xor		r(15), r(15)
jmp		LC7
LC15:
xor		r(17), r(17)
jmp		LC8
LC9:
mov		r(2), r(17)
call		printstr
add		r(17), 1
LC2:
cmp		r(17), 7
jle		LC3
mov		r(2), r(15)
call		printstr
add		r(15), 1
LC4:
cmp		r(15), 6
jle		LC5
mov		r(2), r(13)
call		printstr
add		r(13), 1
LC6:
cmp		r(13), 5
jle		LC7
mov		r(2), r(11)
call		printstr
add		r(11), 1
LC8:
cmp		r(11), 4
jle		LC9
mov		r(2), r(9)
call		printstr
add		r(9), 1
LC10:
cmp		r(9), 3
jle		LC11
mov		r(2), r(7)
call		printstr
add		r(7), 1
LC12:
cmp		r(7), 2
jle		LC13
mov		r(2), r(5)
call		printstr
add		r(5), 1
LC14:
cmp		r(5), 1
jle		LC15
