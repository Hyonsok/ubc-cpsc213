.pos 0x100
start:
	ld $sb, r5              # sb = address of last word of stack
	inca    r5              # sb = address of word after stack
	gpc $6, r6              # r6 = pc + 6
	j main                  # goto main function
	halt                    # halt
	
f:	
	deca r5                 # allocate f's stack
	ld $0, r0               # r0 = i = 0
	ld 4(r5), r1            # r1 = allocate value at &(r5+4)
	ld $0x80000000, r2      # r2 = 8
f_loop:
	beq r1, f_end           # goto f_end if r1 == 0
	mov r1, r3              # r3 = r1
	and r2, r3              # r3 = 0x80000000 & r1
	beq r3, f_if1           # goto f_if1 if r3 == 0
	inc r0                  # r0 = i++
f_if1:
	shl $1, r1              # r1 = r1/2
	br f_loop               # goto f_loop
f_end:
	inca r5                 # r5 = r5 + 4
	j(r6)                   # return
	
main:
	deca r5                 # r5 = r5 - 4
	deca r5                 # r5 = r5 - 4
	st r6, 4(r5)            # &[r5+4] = r6
	ld $8, r4               # r4 = 8
main_loop:
	beq r4, main_end        # goto main_end if r4 == 0
	dec r4                  # r4 = r4--
	ld $x, r0               # r0 = &x
	ld (r0,r4,4), r0        # r0 = x[r4]
	deca r5                 # r5 = r5 - 4
	st r0, (r5)             # r5 = r0
	gpc $6, r6              # set return value
	j f                     # goto f

	inca r5                 # r5 = r5 + 4
	ld $y, r1               # r1 = &y
	st r0, (r1,r4,4)        # y[r4] = r0
	br main_loop            # goto main_loop
main_end:
	ld 4(r5), r6            # r6 = [r5+4]
	inca r5                 # r5 = r5 + 4
	inca r5                 # r5 = r5 + 4
	j (r6)                  # return
	
.pos 0x2000
x:
	.long 1
	.long 2
	.long 3
	.long 0xffffffff
	.long 0xfffffffe
	.long 0
	.long 184
	.long 340057058
	
y:
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	
.pos 0x8000
	# These are here so you can see (some of) the stack contents.
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
sb: 	.long 0

