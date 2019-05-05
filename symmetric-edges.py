# encoding: utf-8
import sys


n = int(sys.stdin.readline())
print(str(n))

for line in sys.stdin:
    inps = line.split()
    src, dst = int(inps[0]), int(inps[1])
    print("%d %d" % (src, dst))
    print("%d %d" % (dst, src))
