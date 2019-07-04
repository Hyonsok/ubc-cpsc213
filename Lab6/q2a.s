.pos 0x0
        ld   $sb, r5                # sb = address of last word of stack
        inca r5                     # sb = address of word after stack
        gpc  $6, r6                 # set return address
        j    0x300                  # call function at 0x300
        halt                        # halt
	
.pos 0x100
        .long 0x00001000
	
.pos 0x200
foo:    ld   0x0(r5), r0            # r0 = arg0
        ld   0x4(r5), r1            # r1 = arg1
        ld   $0x100, r2             # r2 = 0x100
        ld   0x0(r2), r2            # r = value of r2
        ld   (r2, r1, 4), r3        # r3 = m[r2[arg1]]
        add  r3, r0                 # r0 = arg0 = arg0 + m[r2[arg1]]
        st   r0, (r2, r1, 4)        # m[0x100[arg1]] = arg0 + m[r2[arg1]]
        j    0x0(r6)                # return
	
.pos 0x300
foz:    ld   $0xfffffff4, r0        # r0 = -12 = -(size of callee part of b's frame)
        add  r0, r5                 # allocate callee part of b's frame
        st   r6, 0x8(r5)            # store return address to stack

        ld   $0x1, r0               # r0 = 1
        st   r0, 0x0(r5)            # save value of arg0 to stack
        ld   $0x2, r0               # r0 = 2
        st   r0, 0x4(r5)            # save value of arg1 to stack

        ld   $0xfffffff8, r0        # r0 = -8 = -(size of caller part of b's frame)
        add  r0, r5                 # allocate callee part of b's frame

        ld   $0x3, r0               # r0 = 3
        st   r0, 0x0(r5)            # save value of arg0 to stack
        ld   $0x4, r0               # r0 = 4
        st   r0, 0x4(r5)            # save value of arg1 to stack

        gpc  $6, r6                 # set return address
        j    0x200                  # call the function at 0x200

        ld   $0x8, r0               # r0 = 8
        add  r0, r5                 # allocate callee part of b's frame
        ld   0x0(r5), r1            # r1 = a0
        ld   0x4(r5), r2            # r2 = a1

        ld   $0xfffffff8, r0        # r0 = -8 = -(size of callee part of b's frame)
        add  r0, r5                 # allocate callee part of b's frame
        st   r1, 0x0(r5)            # a0 = r1
        st   r2, 0x4(r5)            # a1 = r2
        gpc  $6, r6                 # set return address
        j    0x200                  # goto 0x200

        ld   $0x8, r0               # r0 = 8
        add  r0, r5                 # r5 = r0 + r5
        ld   0x8(r5), r6            # r6 = a2
        ld   $0xc, r0               # r0 = 12
        add  r0, r5                 # r5 = allocate callee part of b's frame
        j    0x0(r6)                # return

.pos 0x1000
        .long 0x00000000         
        .long 0x00000000         
        .long 0x00000000         
        .long 0x00000000         
        .long 0x00000000         
        .long 0x00000000         
        .long 0x00000000         
        .long 0x00000000         
        .long 0x00000000         
        .long 0x00000000
	
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
