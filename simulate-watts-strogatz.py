# encoding: utf-8
import sys
import numpy as np
import networkx as nx
import scipy as sp
from scipy.sparse import spdiags


n = int(sys.argv[1])
m = int(sys.argv[2])
k = int(sys.argv[3])

er = nx.newman_watts_strogatz_graph(n, k, m)

print(er.number_of_nodes())
for edge in er.edges:
    print('%d %d' % (edge[0]+1, edge[1]+1))
    print('%d %d' % (edge[1]+1, edge[0]+1))
