from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
import random
import os
import io
import sys

def make_challenge(x,q):
    # conceil the number x
    # with a key of size q
    # create key
    s   = random.randint(0,2**(8*q)-1) #secure_random(q)
    key = int(0).to_bytes(32-q,byteorder='big')+s.to_bytes(q,byteorder='big')
    # pack number
    xb  = str(x)
    d = 16;
    while(d<len(xb)):
        d = d+16;
    d = d+16
    while(len(xb)<d):
        xb = "0"+xb;
    xbe = xb.encode('utf-8')
    aes = Cipher(algorithms.AES(key), modes.CBC(b'0000000000000000')).encryptor()
    y   = aes.update(xbe) + aes.finalize()
    return y

###### main
x       = int( sys.argv[1] )    # number to be conceiled
q       = int( sys.argv[2] )    # challenge difficulty (number of bits to guess)

y       = make_challenge(x,q)

with open('python/tmp/make_challenge.txt', 'w') as f:
    f.write(y.hex())