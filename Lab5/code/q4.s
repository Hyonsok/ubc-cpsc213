.pos 0x1000



      # avg
avginit:   ld $s, r0       # r0 = &s
	       ld (r0), r0     # r0 = s
	       ld $n, r6       # r6 = &n
		   ld (r6), r6     # r6 = n

avgloop:   beq r6, sort    # go to sort if n = 0
           dec r6          # r6 = n - 1
           ld 20(r0), r1   # r1 = average
	       ld  4(r0), r2   # r2 = grade[0]
	       add r2, r1      # r1 = r1 + grade[0]
           ld  8(r0), r2   # r2 = grade[1]
		   add r2, r1      # r1 = r1 + grade[1]
	       ld 12(r0), r2   # r2 = grade[2]
	       add r2, r1      # r1 = r1 + grade[2]
           ld 16(r0), r2   # r2 = grade[3]
		   add r2, r1      # r1 = r1 + grade[3]
	       shr $2, r1      # r1 = r1 / 4
	       st r1, 20(r0)   # average = r1
		   ld $24, r7      # r7 = 24
		   add r7, r0      # r0 = &current postion + 24
		   br avgloop      # go back to loop



	 # sort
sort:     ld $n, r4       # r4 = &n
		  ld (r4), r4     # r4 = n = i

iloop:	  beq r4, median  # r4 = n = i == 0


          ld $1, r5       # r5 = j
		  mov r4, r6      # r6 = i
		  not r6
		  inc r6          # r6 = -i

		  add r5, r6      # r6 = j - i

jloop:    beq r6, iloopin # finish inner loop if j == i



swap:     ld $s, r0       # r0 = &s
	      ld (r0), r0     # r0 = s1
    	  mov r5, r2      # r2 = j
		  dec r2          # r2 = j-1
		  mov r2, r3      # r3 = j-1
		  shl $4, r2      # r2 = (j-1) * 16
		  shl $3, r3      # r3 = (j-1) * 8
		  add r3, r2      # r2 = (j-1) * 24

		  add r2, r0      # r0 = s1 + (j-1)*24
		  mov r0, r1      # r1 = s1
		  add r7, r1      # r1 = s + 24 = s2

		  ld 20(r0), r2   # r2 = avg of s1
		  ld 20(r1), r3   # r3 = avg of s2
		  not r2
		  inc r2          # r2 = -avg of s1
		  add r2, r3      # r3 = avg of s2 - avg of s1

		  # checking s1 has lower grade, if so, swap
		  bgt r3, jloopin  # go to no swap if s2 > s1

		  ld 0(r0), r2    # r2 = s1.id
		  ld 0(r1), r3    # r3 = s2.id
		  st r3, 0(r0)    # s1.id = s2.id
		  st r2, 0(r1)    # s2.id = s1.id

		  ld 4(r0), r2    # r2 = s1.grade[0]
		  ld 4(r1), r3    # r3 = s2.grade[0]
		  st r3, 4(r0)    # s1.grade[0] = s2.grade[0]
		  st r2, 4(r1)    # s2.grade[0] = s1.grade[0]

		  ld 8(r0), r2    # r2 = s1.grade[1]
		  ld 8(r1), r3    # r3 = s2.grade[1]
		  st r3, 8(r0)    # s1.grade[1] = s2.grade[1]
		  st r2, 8(r1)    # s2.grade[1] = s1.grade[1]

		  ld 12(r0), r2    # r2 = s1.grade[2]
		  ld 12(r1), r3    # r3 = s2.grade[2]
		  st r3, 12(r0)    # s1.grade[2] = s2.grade[2]
		  st r2, 12(r1)    # s2.grade[2] = s1.grade[2]

		  ld 16(r0), r2    # r2 = s1.grade[3]
		  ld 16(r1), r3    # r3 = s2.grade[3]
		  st r3, 16(r0)    # s1.grade[3] = s2.grade[3]
		  st r2, 16(r1)    # s2.grade[3] = s1.grade[3]

		  ld 20(r0), r2    # r2 = s1.avg
		  ld 20(r1), r3    # r3 = s2.avg
		  st r3, 20(r0)    # s1.avg = s2.avg
		  st r2, 20(r1)    # s2.avg = s1.avg

jloopin:  inc r6         # r6 = j - i
          inc r5         # r5 = j++
		  br jloop

iloopin:  dec r4         # r4 = n--
          br iloop



     # median
median:   ld  $n, r0      # r0 = &n
          ld (r0), r0     # r0 = n
		  dec r0          # r0 = n - 1
		  shr $1, r0      # r0 = (n - 1)/2
		  mov r0, r1      # r1 = (n - 1)/2

		  shl $4, r0      # r0 = (n - 1)/2 * 16
		  shl $3, r1      # r1 = (n - 1)/2 * 8
		  add r0,r1       # r1 = (n - 1)/2 * 24

		  ld $s, r2       # r2 = &s
		  ld (r2), r2     # r2 = s
		  add r1, r2      # r2 = r2 + (n - 1)/2 * 24 = address of median student

		  ld 0(r2), r2    # r2 = median student s.id
		  ld $m, r3       # r3 = m
		  st r2, (r3)     # m =  median student s.id

          halt


# global
.pos 0x2000
n:    .long 5     # just one student
m:    .long 4567  # put the answer here
s:    .long s1    # address of the array

# heap
.pos 0x3000
s1:   .long 1234  # student ID
      .long 80    # grade 0
      .long 60    # grade 1
      .long 78    # grade 2
      .long 90    # grade 3
      .long 0     # computed average
s2:   .long 4567  # student ID
      .long 50    # grade 0
      .long 40    # grade 1
      .long 30    # grade 2
      .long 20    # grade 3
      .long 0     # computed average
s3:   .long 8910  # student ID
      .long 30    # grade 0
      .long 25    # grade 1
      .long 35    # grade 2
      .long 10    # grade 3
      .long 0     # computed average
s4:   .long 3213  # student ID
      .long 10    # grade 0
      .long 15    # grade 1
      .long 15    # grade 2
      .long 10    # grade 3
      .long 0     # computed average
s5:   .long 3213  # student ID
      .long 40    # grade 0
      .long 75    # grade 1
      .long 55    # grade 2
      .long 40    # grade 3
      .long 0     # computed average