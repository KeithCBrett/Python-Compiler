xor		r(5), r(5)
jmp		LC2
LC11:

xor		r(7), r(7)
jmp		LC3
LC13:

xor		r(9), r(9)
jmp		LC4
LC13:

xor		r(11), r(11)
jmp		LC5
LC11:

xor		r(13), r(13)
jmp		LC6
LC7:

mov		r(2), r(14)
call		printstr

add		r(14), 1
LC2:
cmp		r(14), 4
jle		LC3

mov		r(2), r(11)
call		printstr

add		r(11), 1
LC4:
cmp		r(11), 3
jle		LC5

mov		r(2), r(9)
call		printstr

add		r(9), 1
LC6:
cmp		r(9), 2
jle		LC7

mov		r(2), r(7)
call		printstr

add		r(7), 1
LC8:
cmp		r(7), 1
jle		LC9

mov		r(2), r(5)
call		printstr

add		r(5), 1
LC10:
cmp		r(5), 0
jle		LC11
