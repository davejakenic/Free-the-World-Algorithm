import os
import sys

def secure_random(L):
    L//=4;
    # returns integer of L bits from system's 
    # hardware random generator
    # (that is allegedly crypto-secure)
    return int.from_bytes(os.urandom(L), byteorder="big")

###### main: create N secure random integers of size L
N = int( sys.argv[1] )    # amount
L = int( sys.argv[2] )    # size L

with open('python/tmp/random_num.txt', 'w') as f:
    for k in range(0,N):
        f.write(str( secure_random(L) ))
        f.write('\n')
