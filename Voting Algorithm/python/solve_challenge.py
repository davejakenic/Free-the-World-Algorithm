from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
import random
import io
import sys

def solve_challenge(y,q):
    # sweep from random start through each key ant attempt decryption
    N = 2**(8*q)
    t = random.randint(0,N-1)
    while(True):
        key = int(0).to_bytes(32-q,byteorder='big')+t.to_bytes(q,byteorder='big')
        aes = Cipher(algorithms.AES(key), modes.CBC(b'0000000000000000')).decryptor()
        xtb = aes.update(y) + aes.finalize();
        try:
            xt  = xtb.decode(encoding="utf-8")
            decodable = True
        except:
            decodable = False
        # if xtb was decodable then count whether there are 16 leading zeros
        if(decodable):
            cracked = True
            for i in range(1,16):
                if(not xt[i]=='0'):
                    cracked = False
                    break
            if(cracked):
                return int(xt)
        t = (t+1) % N

###### main

y       = str( sys.argv[1] )    # number to be reveiled
q       = int( sys.argv[2] )    # challenge difficulty (number of bits to guess)

x       = solve_challenge( bytes.fromhex(y) ,q)

with open('python/tmp/solve_challenge.txt', 'w') as f:
    f.write(str(x))