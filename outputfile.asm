mov		reg(0), 0
mov		reg(1),	5
dec		reg(1)
loop0start:
cmp		reg(0), reg(1)
je		loop0end
mov		reg(2),	reg(0)
mov		rcx,	reg(0)
call		printstr
mov		reg(3),	5
mov		rcx,	reg(3)
call		printstr
inc		reg(0)
jmp		loop0start
loop0end:
