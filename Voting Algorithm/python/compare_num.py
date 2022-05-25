import os
import sys
import math 


###### main: tell whether a>b
a = int(sys.argv[1])
b = int(sys.argv[2])

if(a>b):
    s = 1
else:
    s = 0

with open('python/tmp/compare_num.txt', 'w') as f:
    f.write(str(s))