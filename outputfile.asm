mov		reg(0), 0
mov		reg(1),	5
dec		reg(1)
loop0start:
cmp		reg(0), reg(1)
je		loop0end
mov		reg(2),	reg(0)
mov		rcx,	reg(0)
call		printstr
mov		reg(3), 0
mov		reg(4),	10
dec		reg(4)
loop0start:
cmp		reg(3), reg(4)
je		loop0end
mov		reg(5),	reg(3)
mov		reg(6),	reg(0)
push		reg(5)
push		reg(6)
call		cat_args
mov		rcx	rax
call		printstr
inc		reg(3)
jmp		loop0start
loop0end:
inc		reg(0)
jmp		loop1start
loop1end:
