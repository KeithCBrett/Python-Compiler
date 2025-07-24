xor		r(5), r(5)
jmp		LC2
LC25:

xor		r(7), r(7)
jmp		LC3
LC34:

xor		r(9), r(9)
jmp		LC4
LC41:

xor		r(11), r(11)
jmp		LC5
LC46:

xor		r(13), r(13)
jmp		LC6
LC49:

xor		r(15), r(15)
jmp		LC7
LC50:

xor		r(17), r(17)
jmp		LC8
LC49:

xor		r(19), r(19)
jmp		LC9
LC46:

xor		r(21), r(21)
jmp		LC10
LC41:

xor		r(23), r(23)
jmp		LC11
LC34:

xor		r(25), r(25)
jmp		LC12
LC25:

xor		r(27), r(27)
jmp		LC13
LC14:

mov		r(2), r(28)
call		printstr

add		r(28), 1
LC2:
cmp		r(28), 12
jle		LC3

mov		r(2), r(25)
call		printstr

add		r(25), 1
LC4:
cmp		r(25), 11
jle		LC5

mov		r(2), r(18446744073709551615)
call		printstr

add		r(18446744073709551615), 1
LC6:
cmp		r(18446744073709551615), 10
jle		LC7
mov		r(2), r(21)
call		printstr
add		r(21), 1
LC8:
cmp		r(21), 9
jle		LC9
mov		r(2), r(19)
call		printstr
add		r(19), 1
LC10:
cmp		r(19), 8
jle		LC11
mov		r(2), r(17)
call		printstr
add		r(17), 1
LC12:
cmp		r(17), 7
jle		LC13
mov		r(2), r(15)
call		printstr
add		r(15), 1
LC14:
cmp		r(15), 6
jle		LC15
mov		r(2), r(13)
call		printstr
add		r(13), 1
LC16:
cmp		r(13), 5
jle		LC17
mov		r(2), r(11)
call		printstr
add		r(11), 1
LC18:
cmp		r(11), 4
jle		LC19
mov		r(2), r(9)
call		printstr
add		r(9), 1
LC20:
cmp		r(9), 3
jle		LC21
mov		r(2), r(7)
call		printstr
add		r(7), 1
LC22:
cmp		r(7), 2
jle		LC23
mov		r(2), r(5)
call		printstr
add		r(5), 1
LC24:
cmp		r(5), 1
jle		LC25
