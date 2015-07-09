import sys
from random import randint

if len(sys.argv) != 2:
    print "Usage: generate.py <number of orders>"
    sys.exit(1)

n = int(sys.argv[1])

for i in range(0, n):
    action = 'A'
    orderid = i + 1
    side = 'S' if (randint(0,1) == 0) else 'B'
    quantity = randint(1,100)
    price = randint(100,200)

    print action + ',' + str(orderid) + ',' + side + ',' + str(quantity) + ',' + str(price)

