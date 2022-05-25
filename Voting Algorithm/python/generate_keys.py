import random
import os
import sys

from os import listdir
from os.path import isfile, join

def secure_random(L):
    L//=4;
    # returns integer of L bits from system's 
    # hardware random generator
    # (that is allegedly crypto-secure)
    return int.from_bytes(os.urandom(L), byteorder="big")

def test_prime(n):
    d = int(n-1)
    r = 0;
    while(d%2==0):
        d //= 2
        r = r+1
    for k in range(1,10):
        a = random.randint(2,n-2)
        x = pow(a,d,n)
        if(x==1 or x==n-1):
            break
        for kr in range(1,r-1):
            x = x*x % n;
            if(x==n-1):
                break
        if(x==n-1):
            break
        else:
            return False
    return True

def get_prime(L):
    while(True):
        n = secure_random(L)
        if(test_prime(n)):
            return n

def get_rsa_keys(L):
    e = get_prime(1*L)
    p = get_prime(2*L)
    q = get_prime(3*L)
    n = p*q
    t = (p-1)*(q-1)
    d = pow(e,-1,t)
    return ((d,n),(e,n))

###### main
L       = int( sys.argv[1] )
keys    = get_rsa_keys(L)

with open('python/tmp/generate_keys.txt', 'w') as f:
    f.write(str(keys[0][0]))
    f.write('\n')
    f.write(str(keys[0][1]))
    f.write('\n')
    f.write(str(keys[1][0]))
    f.write('\n')
    f.write(str(keys[1][1]))
