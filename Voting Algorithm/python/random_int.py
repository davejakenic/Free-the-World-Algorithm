import random
import sys

###### main: create N uniformly random integers in [i0,iE)
N       = int( sys.argv[1] )    # amount
i0      = int( sys.argv[2] )    # left end
iE      = int( sys.argv[3] )    # right end

with open('python/tmp/random_int.txt', 'w') as f:
    for k in range(0,N):
        f.write(str( random.randint(i0,iE-1) ))
        f.write('\n')