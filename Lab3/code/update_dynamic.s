.pos 0x100

            #allocate t
            ld $t,r0                # r0 = address of t
            ld (r0),r1              # r1 = value of t

            #allocate array
            ld $array,r2            # r2 = address of array
            ld (r2),r2              # r2 = value of array

            #t = array[2]
            ld $0x2,r7              # r7 = 2
            ld (r2,r7,4),r3         # r3 = array[2]
            mov r3,r1               # r1 = r3
            st r1,(r0)              # store r0 = t = array[2]

            #t += 4
            ld $0x4,r7              # r7 = 4
            add r7,r1               # r1 = t = t+4
            st r1,(r0)              # store t += 4

            #array[1] = array[2] + t
            mov r3,r4               # r4 = r3
            add r1,r4               # r4 += r1
            ld $0x1,r7              # r7 = 1
            st r4,(r2,r7,4)         # store array[1] = array[2]+t

            #array[0] = t
            ld $0x0,r7                 # r7 = 0
            st r1,(r2,r7,4)            # array[0] = t

            halt                    #halt

.pos 0x1000
t:              .long 0x0
array:          .long 0x2000

.pos 0x2000
array_data:     .long 0x1
                .long 0x2
                .long 0x3

