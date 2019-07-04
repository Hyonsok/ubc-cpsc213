.pos 0x100
ld  $a, r0
ld  $b, r1
st  r0, (r1)
halt

.pos 0x1000
a:  .long 1
b:  .long 2