.pos 0x100
start:	
        ld   $sb, r5	# [01] sp = address of last word of stack
        inca r5			# [02] so = address of word after stack
        gpc  $6, r6     # [03] r6 = pc + 6
        j    main		# [04] call main
        halt

.pos 0x200
main:	
	deca r5				# [05] allocate caller part of main's frame
	st   r6, (r5)		# [06] save ra on stack
	ld   $-12, r1		# [07] r1 = -12 = (size of caller part of search's frame)
	add  r1, r5			# [08] allocate caller part of search's frame
	ld   $a, r0			# [09] r0 = &a
	st   r0, 0x0(r5)	# [10] save &a in stack
	ld   $val, r0		# [11] r0 = &val
	ld   (r0), r0		# [12] r0 = val
	st   r0, 0x4(r5)	# [13] save val in stack
	ld   $size, r0		# [14] r0 = &size
	ld   (r0), r0		# [15] r0 = size
	st   r0, 0x8(r5)	# [16] save size in stack
	gpc  $6, r6			# [17] r6 = pc + 6
	j    search			# [18] go to search
	ld   $12, r1		# [19] r1 = 12 = size of search part of 's frame
	add  r1, r5			# [20] deallocate caller part of 's frame
	ld   $ret, r1		# [21] r1 = &ret
	st   r0, (r1)		# [22] ret = size2 + &a
	ld   (r5), r6		# [23] load ra from stack
	inca r5				# [24] remove callee part of stack frame
	j    (r6)			# [25] return
	
.pos 0x300
search:	
	deca r5				# [01] allocate caller part of search's frame'
	st   r6, (r5)		# [02] save ra on stack
	ld   0xC(r5), r0	# [03] r0 = size
	beq  r0, L3			# [04] if r0 == 0, go to L3
	ld   0x4(r5), r1	# [05] r1 = &a
	ld   0x8(r5), r2	# [06] r2 = value
	mov  r0, r3			# [07] r3 = size1
	shr  $1, r3			# [08] r3 = size1 / 2
	mov  r3, r4			# [09] r4 = size2 = size1 / 2
	shl  $2, r4			# [10] r4 = size2 = size1 * 2
	add  r1, r4			# [11] r4 = size2 + &a
	ld   (r4), r7		# [12] r7 = value of &(size2 + &a)
	not  r7				# [13] r7 = !value of &(size2 + &a)
	inc  r7				# [14] r7 = -value of &(size2 + &a)
	add  r2, r7			# [15] r7 = value - value of &(size2 + &a)
	beq  r7, L2			# [16] if value == value of &(size2 + &a), go to L2
	bgt  r7, L0			# [17] if value > value of &(size2 + &a), go to L0
	br   L1				# [18] go to L1
L0:
	mov  r4, r1			# [01] r1 = size2 + &a
	inca r1				# [02] r1 = size2 + &a + 4
	not  r3				# [03] r3 = !(size1 / 2)
	add  r0, r3			# [04] r3 = size + !(size1 / 2)
L1: #tail recursive fuction to iterate function
	ld   $-12, r4		# [01] r4 = - 12
	add  r4, r5			# [02] allocate caller part of L1's frame
	st   r1, 0x0(r5)	# [03] store r1 = &a to stack
	st   r2, 0x4(r5)	# [04] store r2 = value to stack
	st   r3, 0x8(r5)	# [05] store r3 = size1 / 2 to stack
	gpc  $6, r6			# [06] save ra on stack
	j    search			# [07] go to search
	ld   $12, r1		# [08] r1 = 12
	add  r1, r5			# [09] deallocate callee part of L1's frame
	br   L3				# [10] go to L3
L2:			
	mov  r4, r0			# [01] r0 = size2 + &a
L3:
	ld   (r5), r6		# [01] r6 = return address
	inca r5				# [02] r5 = deallocate callee part of search's frame
	j    (r6)			# [03] return

.pos 0x1000
a:	.long 0x1
        .long 0x3         
        .long 0x4
	.long 0x7         
	.long 0x9         
        .long 0xA         
        .long 0xC         
	.long 0x10         
	.long 0x12         
        .long 0x13
	.long 0x14
	.long 0x17
	.long 0x1A
	.long 0x1B
	.long 0x1F
	
size:	.long 15
val:	.long 0x12
ret:	.long 0xFFFFFFFF
	
.pos 0x8000
stack:	
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
