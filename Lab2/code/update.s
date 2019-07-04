.pos 0x100
                 ld   $t, r0                  # r0 = address of t
                 ld   (r0),r1                 # r1 = value of t

                 ld   $array, r2              # r2 = address of array

                 ld   $0x2, r3                # r3 = 2
                 ld   (r2,r3,4), r7           # r7 = array[2]
                 mov  r7,r1                   # r1 = r7 (t = array[2])

                 ld    $0x4, r3               # r3 = 4
                 add   r3,r1                  # r1 += 4 (t = t+4)

                 ld    $0x1,r3                # r3 = 1
                 ld    (r2,r3,4),r6           # r6 = array[1]

                 add   r1,r7                  # r7 += r1
                 mov   r7,r6                # array[1] = array[2]+t

                 ld    $0x2, r3             # r3 = 2
                 ld   (r2,r3,4),r7          # r7 = array[2]

                 ld    $0x0, r3               # r3 = 0
                 ld    (r2,r3,4),r5           # r5 = array[0]
                 mov   r1,r5                  # array[0] = t

                  st    r1,(r0)                # t += 4

                  ld    $0x0,r3                 # r3 =0
                  st    r5, (r2,r3,4)

                  ld    $0x1,r3                 # r3=1
                  st    r6,(r2,r3,4)

                  ld    $0x2,r3                 # r3 =2

                  st    r7,(r2,r3,4)

                  halt                         # halt
.pos 0x1000
t:               .long 0x0
.pos 0x2000
array:           .long 0x0
                 .long 0x1
                 .long 0x2
