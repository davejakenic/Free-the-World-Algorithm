import os
import sys
import math 


###### main: tell whether a>b
a = int(sys.argv[1])
b = int(sys.argv[2])

x = a+b;

with open('python/tmp/add_num.txt', 'w') as f:
    f.write(str(x))