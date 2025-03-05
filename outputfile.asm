xor		reg(0),	reg(0)
mov		reg(1),	7
mov		reg(0),	reg(1)
xor		reg(2),	reg(2)
mov		reg(3),	2
mov		reg(2),	reg(3)
mov		reg(4),	reg(1)
mov		reg(5),	reg(3)
mul		reg(4),	reg(5)
mov		rcx,	reg(4)
call		printstr
