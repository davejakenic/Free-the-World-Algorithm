import hashlib
import random
import sys

def proof_of_work(s,q):
    k = random.randint(0,16**q-1)
    while(True):
        k = k+1
        h = hashlib.sha256()
        h.update( (s + hex(k)).encode('utf8') )
        t = str(h.hexdigest())
        # if td was decodable then count whether there are q leading zeros
        cracked = True
        for i in range(0,q):
            if(not t[i]=='0'):
                cracked = False
                break
        if(cracked):
            return k



###### main
s       = str( sys.argv[1] )    # digest
q       = int( sys.argv[2] )    # pow difficulty (number of leading zeros)

k       = proof_of_work(s,q)

with open('python/tmp/proof_of_work.txt', 'w') as f:
    f.write(str(k))