xor		r(5), r(5)
mov		r(6), 5
dec		r(6)
FLB0:
cmp		r(5), r(6)
je		FLE0
FLM0:
mov		r(2), r(5)
call		printstr
mov		r(7), 5
mov		r(2), r(7)
call		printstr
