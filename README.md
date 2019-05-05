# Graph Spectral Moments calculator

## Constraints

1. **Undirected** graph;
2. **Unweighted** graph;
3. Graph with **no loops**;
4. Graph with **no multiple edges** for the same pair of nodes;

## Input format

First, input the number of nodes of the graph. Then, input its edges.

```
[number-of-nodes]
[source-node] [destination-node]
[source-node] [destination-node]
[source-node] [destination-node]
...
```

For the algorithm to work, the edges input must be symmetric. For example, if the input has an edge `6 10`, so there must be another edge input `10 6`.

## Compile and run

1. You can compile with GCC compiler
```bash
gcc spectral-moments.c -o spectral-moments
```
2. If you are using linux, grant the execution permission 
```bash
chmod +x spectral-moments
```
3. Run program. Where `[spectral-moments]` is the number of first spectral moments to calculate, defaulted to 10.
```bash
./spectral-moments [spectral-moments] < input-file.txt
```

## For non-symmetric inputs

You can convert it to symmetric.

1. You must install [Python](https://www.python.org/)
2. Run the program
```bash
python3 symmetric-edges.py < non-symmetric-input.txt | ./spectral-moments [spectral-moments]
```
