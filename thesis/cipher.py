from operator import xor
from random import randrange
from random import seed



def createSbox():
	sbox = {}
	sbox[0x0] = 0xe
	sbox[0x1] = 0x4
	sbox[0x2] = 0xd
	sbox[0x3] = 0x1
	sbox[0x4] = 0x2
	sbox[0x5] = 0xf
	sbox[0x6] = 0xb
	sbox[0x7] = 0x8
	sbox[0x8] = 0x3
	sbox[0x9] = 0xa
	sbox[0xa] = 0x6
	sbox[0xb] = 0xc
	sbox[0xc] = 0x5
	sbox[0xd] = 0x9
	sbox[0xe] = 0x0
	sbox[0xf] = 0x7
	print(sbox)
	return sbox

def createSboxInv():
	sbox = {}
	sbox[0xe] = 0x0
	sbox[0x4] = 0x1
	sbox[0xd] = 0x2
	sbox[0x1] = 0x3
	sbox[0x2] = 0x4
	sbox[0xf] = 0x5
	sbox[0xb] = 0x6
	sbox[0x8] = 0x7
	sbox[0x3] = 0x8
	sbox[0xa] = 0x9
	sbox[0x6] = 0xa
	sbox[0xc] = 0xb
	sbox[0x5] = 0xc
	sbox[0x9] = 0xd
	sbox[0x0] = 0xe
	sbox[0x7] = 0xf
	return sbox

def createDistTable():
	Table = [[0 for x in range(16)] for y in range(16)]

	for targetDiff in range(16):
		for currentNum in range(16):
			diff = xor(currentNum,targetDiff)
			c1 = sbox[currentNum]
			c2 = sbox[diff]
			diff = xor(c1,c2)
			Table[targetDiff][diff] +=1

		
	for i in range(16):
		print(Table[i])
	return Table

def round_enc(message,key,sbox):
	m = xor(message,key)
	return sbox[m]
def encrypt(message,subkey1,subkey2,subkey3,sbox):
	m = round_enc(message,subkey1,sbox)
	m = round_enc(m,subkey2,sbox)
	m = xor (m,subkey3)
	return m
def round_dec(message,key,invbox):
	m = invbox[message]
	return xor(m,key)
def decrypt(message,subkey1,subkey2,subkey3,invbox):
	m = xor(message,subkey3)
	m = round_dec(m,subkey2,invbox)
	return round_dec(m,subkey1,invbox)


sbox = createSbox()
sboxinv = createSboxInv()
createDistTable()
subkey1 = randrange(16)
subkey2 = randrange(16)
subkey3 = randrange(16)

goodpairs = []
diffTarget = 0xb
for p1 in range(16):
	p2 = xor(p1,diffTarget)
	c1 = encrypt(p1,subkey1,subkey2,subkey3,sbox)
	c2 = encrypt(p2,subkey1,subkey2,subkey3,sbox)
	goodpairs.append([p1,p2,c1,c2])


keycandidate = [0 for x in range(16)]
#breaking round 3
for key in range(16):
	for pair in goodpairs:
		u1 = sboxinv[xor(key,pair[2])]
		u2 = sboxinv[xor(key,pair[3])]
		if( xor(u1,u2) == 0x2):
			keycandidate[key] += 1
print(keycandidate)
subkey3guess = max(range(len(keycandidate)), key = lambda x: keycandidate[x])
keycandidate = [0 for x in range(16)]
print("Subkey used: "+bin(subkey3))
print("Subkey Guessed: "+bin(subkey3guess))
#breaking round 2
for key in range(16):
	for pair in goodpairs:
		u1 = sboxinv[xor(subkey3guess,pair[2])]
		u2 = sboxinv[xor(subkey3guess,pair[3])]
		u1 = sboxinv[xor(key,u1)]
		u2 = sboxinv[xor(key,u2)]
		if( xor(u1,u2) == 0xb):
			keycandidate[key] += 1
subkey2guess = max(range(len(keycandidate)), key = lambda x: keycandidate[x])
print("Subkey used: "+bin(subkey2))
print("Subkey Guessed: "+bin(subkey2guess))
#breaking round 1
c1 = goodpairs[0][2]
c1 = sboxinv[xor(subkey3guess,c1)]
c1 = sboxinv[xor(subkey2guess,c1)]
subkey1guess = xor(goodpairs[0][0],c1)
print("Subkey used: "+bin(subkey1))
print("Subkey Guessed: "+bin(subkey1guess))

