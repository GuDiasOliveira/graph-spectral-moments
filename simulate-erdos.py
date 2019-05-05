# encoding: utf-8
import sys
import numpy as np
import networkx as nx
import scipy as sp
from scipy.sparse import spdiags


n = int(sys.argv[1])
p = int(sys.argv[2])

er = nx.erdos_renyi_graph(n,p=p)

print(er.number_of_nodes())
for edge in er.edges:
    print('%d %d' % (edge[0]+1, edge[1]+1))
    print('%d %d' % (edge[1]+1, edge[0]+1))
