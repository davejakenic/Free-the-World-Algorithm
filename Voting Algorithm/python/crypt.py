import sys

def crypt(x,key):
    return pow(x,key[0],key[1])

###### main

fprint = False

if(fprint):
    print(".................................................................")
    print("x : ", sys.argv[1] )
    print("d : ", sys.argv[2] )
    print("n : ", sys.argv[3] )

x       = int( sys.argv[1] )
d       = int( sys.argv[2] )
n       = int( sys.argv[3] )
key     = (d,n)

y       = crypt(x,key)


if(fprint):
    print("y : ",y)
    print("°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°")


with open('python/tmp/crypt.txt', 'w') as f:
    f.write(str(y))