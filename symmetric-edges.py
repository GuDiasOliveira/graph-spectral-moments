# encoding: utf-8
import sys


n = int(sys.stdin.readline())
print(str(n))

for line in sys.stdin:
    inps = line.split()
    src, dst, weight = int(inps[0]), int(inps[1]), float(inps[2])
    print("%d %d %f" % (src, dst, weight))
    print("%d %d %f" % (dst, src, weight))
