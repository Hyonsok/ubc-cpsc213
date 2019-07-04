.pos 0x1000
code:
                # v0 = s.x[i]
                ld $v0,r0           # r0 = address of v0
                ld (r0),r1          # r1 = value of v0
                ld $s,r2            # r2 = address of s_x
                ld $i,r3            # r3 = address of i
                ld (r3),r3          # r4 = value of i
                ld (r2,r3,4),r1     # r1 = s.x[i]
                st r1,(r0)          # store value into memory

                # v1 = s.y[i]
                ld $v1,r0           # r0 = address of v1
                ld (r0),r1          # r1 = value of v1
                ld $s_y,r2          # r2 = address of s_y
                ld $i,r3            # r3 = address of i
                ld (r3),r3          # r4 = value of i
                ld (r2,r3,4),r1     # r1 = s.y[i]
                st r1,(r0)          # store value into memory

                # v2 = s.z -> x[i]
                ld $v2,r0           # r0 = address of v2
                ld (r0),r1          # r1 = value of v2
                ld $s_z,r2          # r2 = address of s_z_x
                ld $i,r3            # r3 = address of i
                ld (r3),r3          # r4 = value of i
                ld (r2,r3,4),r1     # r1 = s.z->x[i]
                st r1,(r0)          # store value

                # v3 = s.z->z->y[i]
                ld $v3,r0           # r0 = address of v3
                ld (r0),r1          # r1 = value of v3
                ld $s_z_z_y,r2      # r2 = adress of s_z_z_y
                ld (r2,r3,4),r1     # r1 = x.z->z->y[i]
                st r1,(r0)

                halt



.pos 0x2000
static:

i:              .long 0

v0:             .long 0
v1:             .long 0
v2:             .long 0
v3:             .long 0

s:              .long 1                # s.x[0]
                .long 0                # s.x[1]
                .long s_y
                .long s_z


.pos 0x3000
heap:
s_y:            .long 1                # s.y[0]
                .long 2                # s.y[1]

s_z:            .long 3                # s.z->x[0]
                .long 4                # s.z->x[1]
                .long s_z_y            # s.z->y
                .long s_z_z            # s.z->z

s_z_y:          .long 1
                .long 2

s_z_z:          .long 6                # s.z->z->x[0]
                .long 7                # s.z->z->x[1]
                .long 8                # s.z->z->z
                .long s_z_z_y          # s.z->z>y


s_z_z_y:        .long 4                # s.z->z->y[0]
                .long 3                # s.z->z->y[1]


