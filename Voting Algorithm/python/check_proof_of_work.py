import hashlib
import random
import sys

def check_proof_of_work(s,k,q):
    h = hashlib.sha256()
    h.update( (s + hex(k)).encode('utf8') )
    t = str(h.hexdigest())
    for i in range(0,q):
        if(not t[i]=='0'):
            return False
    return True


###### main
s       = str( sys.argv[1] )    # digest
k       = int( sys.argv[2] )    # pow solution
q       = int( sys.argv[3] )    # pow difficulty (number of leading zeros)

b       = check_proof_of_work(s,k,q)
if(b):
    b   = int(1) #true
else:
    b   = int(0) #false

with open('python/tmp/check_proof_of_work.txt', 'w') as f:
    f.write(str(b))