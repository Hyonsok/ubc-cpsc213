.pos 0x100
                 ld   $a, r0              # r0 = address of a
                 ld   (r0), r1            # r1 = value of a
                 ld   $b, r2              # r2 = address of b
                 ld   (r2), r3            # r3 = value of b
                 mov  r3,r4               # r4 = r3
                 inc  r4                  # r4++
                 mov  r3,r5               # r5 = r3
                 shr  $1,r5               # b/2
                 add  r4,r5               # r5 +=r4
                 and  r3,r5               # r3 and r5
                 shl  $2,r5               # shift left by 2
                 mov  r5,r1               # r1 = r5
                 st   r1,(r0)             # store and address
                 halt                     # halt
.pos 0x1000
a:               .long 0x0
.pos 0x2000
b:               .long 0x8