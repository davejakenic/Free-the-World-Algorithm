import os
import sys
import math 


###### main: create N secure random integers of size L
s = sys.argv[1]

i = int(s,16)

L = math.ceil( math.log2(i) )

with open('python/tmp/hex_to_num.txt', 'w') as f:
    f.write(str( i ))
    f.write(str( L ))