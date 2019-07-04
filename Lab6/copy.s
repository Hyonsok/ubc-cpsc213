.pos 0x100
main:
	ld $sb, r5              # sb = address of last word of stack
	inca    r5              # sb = address of word after stack
	gpc $6, r6              # r6 = pc + 6
	j copy                  # goto main function
	halt                    # halt

.pos 0x200
copy:   deca r5             # allocate the frame of copy
        st r6,(r5)          # store ra
        deca r5             # dst[0]
        deca r5             # dst[1]
        ld $0, r0           # r0 = 0
        ld $src,r1          # r1 = &src

        cond:
        ld (r1,r0,4),r2     # r2 = src[i]
        beq r2,end_loop     # goto end_loop if src[i] == 0

        loop:
        st r2,(r5,r0,4)     # dst[i] = src[i]
        inc r0
        br cond

        end_loop:
        ld $0,r7
        st r7,(r5,r0,4)
        ld 8(r5), r6            # r6 = [r5+4]
        ld $8,r7
        add r7,r5
        j 0(r6)

.pos 0x1000
src:    .long 0x1
    	.long 0x2
    	.long 0x0000201c
    	.long ffff0100
    	.long ffffffff
    	.long ffff0200
    	.long ffffffff
    	.long ffff0300
    	.long ffffffff
    	.long ffff0400
    	.long ffffffff
    	.long ffff0500
    	.long ffffffff
    	.long ffff0600
    	.long ffffffff
    	.long ffff0700
    	.long ffffffff
    	.long ffff0800
    	.long ffffffff

.pos 0x2000
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
sb: 	.long 0
