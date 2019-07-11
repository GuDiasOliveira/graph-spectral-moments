# encoding: utf-8
import sys
import numpy as np
import networkx as nx
import scipy as sp
from scipy.sparse import spdiags
from random import uniform


n = int(sys.argv[1])
m = int(sys.argv[2])

er = nx.barabasi_albert_graph(n, m)

print(er.number_of_nodes())
for edge in er.edges:
    weight = uniform(1, 6)
    print('%d %d %f' % (edge[0]+1, edge[1]+1, weight))
    print('%d %d %f' % (edge[1]+1, edge[0]+1, weight))
