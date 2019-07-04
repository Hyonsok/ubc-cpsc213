.pos 0x100
                 ld   $sum, r0              # r0 = address of sum
                 ld   (r0), r1              # r1 = value of sum
                 ld   $and, r2              # r2 = address of and
                 ld   (r2), r3              # r3 = value of and
                 ld   $b  , r4              # r4 = address of b (which stores address)
                 ld   $0x0 , r5             # r5 = 0
                 ld   (r4,r5,4),r1          # r1 = b[0]
                 mov  r1,r6                 # r6 = r1
                 ld   $0x1, r5              # r5 = 1
                 ld   (r4,r5,4),r3          # r3 = b[1]
                 add  r3,r1                 # r1 = r1+b[1]
                 st   r1,(r0)               # store new value of sum
                 and  r6,r3                 # b[0] and b[1]
                 st   r3, (r2)              # store and value
                 halt                       # halt

.pos 0x1000
sum:             .long 0x0                  # sum
.pos 0x2000
and:             .long 0x0                  # and
.pos 0x3000
b:               .long 0x1
                 .long 0x2

