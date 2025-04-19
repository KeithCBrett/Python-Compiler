xor		r(5), r(5)
mov		r(6), 5
dec		r(6)
FLB0:
cmp		r(5), r(6)
je		FLE0
FLM0:
mov		r(2), r(5)
call		printstr
inc		r(5)
jmp		FLB0
FLE0:
