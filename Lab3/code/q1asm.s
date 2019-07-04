.pos 0x100
	# j = 3;
	ld $j, r0         # r0 = address of j
	ld $0x3, r1       # r1 = 3
	st r1, 0(r0)      # 0(r0) = value of j = 3

	# i  = a[j];
	ld $i, r0         # r0 = address of i
	ld $a, r1         # r1 = address of a
	ld $j, r2         # r2 = address of j
	ld 0(r2), r3      # r3 = j
	ld (r1, r3, 4), r4 # r4 = a[j]
	st r4, 0(r0)      # 0(r1) = value of i = a[j]

	# i  = i & 9;
	ld $i, r0         # r0 = address of i
	ld $0x9, r1         # r1 = 9
	ld 0(r0), r2      # r2 = i
	and r1, r2        # r2 = i & 9
	st r2, 0(r0)      # i = i & 9

	# i  = a[i];
	ld $i, r0         # r0 = address of i
	ld $a, r1         # r1 = address of a
	ld 0(r0), r2      # r2 = i
	ld (r1, r2, 4), r3 # r3 = a[i]
	st r3, 0(r0)      # 0(r0) = value of i = a[i]

	# p  = &j;
	ld $p, r0         # r0 = address of p
	ld $j, r1         # r1 = address of j
	st r1, 0(r0)         # 0(r0) = address of j

	#j++;
	ld $j, r0         # r0 = address of j
	ld 0(r0), r1      # r1 = j
	inc r1            # r1 = j + 1
	st r1, 0(r0)      # 0(r0) = j + 1

	# *p = *p - 2;
	ld $p, r0         # r0 = address of p
	ld 0(r0), r1      # r1 = address of j
	ld 0(r1), r2      # r2 = j
	dec r2            # r2 = r2 - 1;
	dec r2            # r2 = r2 - 2;
	st r2, 0(r1)      # 0(r1) = r2

	# p  = &a[a[j]];
	ld $p, r0        # r0 = address of p
	ld $a, r1        # r1 = address of a
	ld $j, r2        # r2 = address of j
	ld 0(r2), r3     # r3 = j
	shl $2, r3       # r3 = j * 4
	add r3, r1       # r1 = a + j * 4 = address of a[j]
	ld 0(r1), r4     # r4 = a[j]
	shl $2, r4       # r4 = a[j] * 4
	ld $a, r1        # r1 = address of a
	add r4, r1       # r1 = address of a + a[j] * 4
	st r1, 0(r0)     # p = &a[a[j]]

	# *p = *p + a[4];
	ld $p, r0         # r0 = address of p
	ld 0(r0), r1      # r1 = address of a[a[j]]
	ld 0(r1), r2      # r2 = a[a[j]]

	ld $a, r3         # r3 = address of a
	ld $0x4, r4       # r4 = 4
	ld (r3, r4, 4), r5 # r5 = a[4]

	add r5, r2        # r2 = a[a[j]] + a[4]
	st r2, 0(r1)      # a[a[j]] = a[a[j]] + a[4]

	halt

.pos 0x200

i:  .long 0              # i
j:  .long 0              # j
p:  .long 0              # p
a:  .long 0              # a[0]
    .long 0              # a[1]
	.long 0              # a[2]
	.long 0              # a[3]
	.long 0              # a[4]
	.long 0              # a[5]
	.long 0              # a[6]
	.long 0              # a[7]
	.long 0              # a[8]
	.long 0              # a[9]

