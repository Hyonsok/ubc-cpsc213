.pos 0x100
                ld $tmp, r0             # r0 = address of tmp
                ld $0x0, r7             # r7 = 0
                st r7, (r0)             # store: tmp = 0
                ld (r0), r1             # r1 = value of tmp

                ld $tos, r2             # r2 = address of tos
                st r7,  (r2)            # store: toss = 0
                ld (r2),r3              # r3 = value of tos

                #a[0] = s[tos]+tos
                ld $s,   r6             # r6 = address of s
                ld (r6), r6             # r6 = value of s
                ld (r6,r3,4),r5         # r5 = s[tos]
                add r3,  r5             # r5 += r3
                ld $a,   r4             # r4 = address of a
                st r5,  (r4,r7,4)       # a[0] = s[tos] + tos

                #a[1] = s[tos]+tos
                inc r3                  # r3 = tos++
                ld (r6,r3,4),r5         # r5 = s[tos]
                add r3,r5               # r5 += r3
                ld $0x1,r7              # r7 = 1
                st r5,(r4,r7,4)         # a[1] = s[tos] + tos

                #a[2] = s[tos]+tos
                inc r3                  # r3 = tos++
                ld (r6,r3,4),r5         # r5 = s[tos]
                add r3,r5               # r5 += r3
                ld $0x2,r7              # r7 = 2
                st r5, (r4,r7,4)        # a[2] = s[tos] + tos

                inc r3                  # tos++
                dec r3                  # tos--

                ld  (r4,r3,4),r1        # tmp = a[tos]
                dec r3                  # tos--
                ld (r4,r3,4),r5         # r6 = a[tos]
                add r5,r1               # tmp = tmp+a[tos]
                dec r3                  # tos--
                ld (r4,r3,4),r5         # r6 = tmp + a[tos]
                add r5,r1               # tmp = tmp+a[tos]
                st r1,(r0)              # store tmp = tmp + a[tos]

                ld $0x0,r7              # r7 = 0
                st r1,(r6,r7,4)         # s[0] = tmp

                halt                    # halt

.pos 0x2000
a:              .long 0x0
                .long 0x0
                .long 0x0
s:   .long 0x3000
tos:            .long 0x0
tmp:            .long 0x0

.pos 0x3000
s_ar:           .long 0x0
                .long 0x0
                .long 0x0
                .long 0x0
                .long 0x0
