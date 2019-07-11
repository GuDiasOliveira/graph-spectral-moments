#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct node * Node;
typedef struct graph * Graph;
typedef struct edge * Edge;

struct graph {
    Node nodes;
    long N;
};

struct edge {
    long src;
    long dst;
    double w;
};

struct node {
    Edge edges;
    long degree;
    long capacity;
    double Wmax;
};

double frandom() {
    static char init = 0;
    if (!init) {
        // This will be done just once
        srand(time(NULL));
        init = 1;
    }
    return (double) rand() / ((long) RAND_MAX + 1);
}
double frandom_range(double start, double end) {
    double diff = frandom() * (end - start);
    return start + diff;
}

Graph readGraph() {
    Graph g = malloc(sizeof(struct graph));
    Edge newEdges;
    long i;
    long n = 0;
    long src, dst;
    int reading = 1;
    double weight;
    Edge node;
    fscanf(stdin, "%ld", &n);
    g->N = n;
    g->nodes = malloc(sizeof(struct node) * (n + 1));
    for (i = 0; i <= n; i++) {
        g->nodes[i].degree = 0;
        g->nodes[i].capacity = 100;
        g->nodes[i].edges = malloc(sizeof(struct edge) * g->nodes[i].capacity);
        g->nodes[i].Wmax = 0;
    }
    while (reading) {
        reading = fscanf(stdin, "%ld", &src);
        if (reading < 0) break;
        reading = fscanf(stdin, "%ld", &dst);
        if (reading < 0) break;
        reading = fscanf(stdin, "%lf", &weight);
        if (reading < 0) break;
        if (g->nodes[src].degree >= g->nodes[src].capacity) {
            g->nodes[src].capacity = 2 * g->nodes[src].capacity;
            newEdges = malloc(sizeof(struct edge) * g->nodes[src].capacity);
            for (i = 0; i < g->nodes[src].degree; i++) {
                newEdges[i].src = g->nodes[src].edges[i].src;
                newEdges[i].dst = g->nodes[src].edges[i].dst;
                newEdges[i].w = g->nodes[src].edges[i].w;
            }
            free(g->nodes[src].edges);
            g->nodes[src].edges = newEdges;
        }
        g->nodes[src].edges[g->nodes[src].degree].src = src;
        g->nodes[src].edges[g->nodes[src].degree].dst = dst;
        g->nodes[src].edges[g->nodes[src].degree].w = weight;
        if (weight > g->nodes[src].Wmax)
            g->nodes[src].Wmax = weight;
        g->nodes[src].degree += 1;

    }
    return g;
}

void normalizeWeights(Graph g) {
    long i, j;
    double sum = 0;
    for (i = 0; i < g->N; i++) {
        sum = 0;
        for (j = 0; j < g->nodes[i].degree; j++) {
            sum += g->nodes[i].edges[j].w;
        }
        for (j = 0; j < g->nodes[i].degree; j++) {
            g->nodes[i].edges[j].w /= sum;
        }
    }
}

# define s 10000

long chooseNextNode(Graph g, long src) {
    long firstStep;
    double second;
    double threshold;
    int count = 0;
    while (1) {
        firstStep = frandom_range(0, g->nodes[src].degree);
        second = frandom();
        threshold = g->nodes[src].edges[firstStep].w / g->nodes[src].Wmax;
        if (second < threshold)
            return g->nodes[src].edges[firstStep].dst;
        count++;
        if (count > 1000)
            return g->nodes[src].edges[firstStep].dst;
    }

}

double * approxSpectralMoments(Graph g, long l) {
    long i, k;
    long w, j;
    double * x = malloc(sizeof(double) * (l + 1));
    for (i = 0; i <= l; i++)
        x[i] = 0;
    for (i = 0; i < s; i++) {
        w = j = (long) frandom_range(1, g->N + 1);
        for (k = 0; k <= l; k++) {
            w = chooseNextNode(g, w);
            if (w == j)
                x[k] += 1.0 / s;
        }
    }
    return x;
}

int main(int argc, char** argv) {
    int i;
    long l = argc >= 2 ? atoi(argv[1]) : 10;
    Graph g = readGraph();
    long runStart, runEnd;
    double * moments;
    normalizeWeights(g);

    printf("Calculating...\n");
    runStart = clock();
    moments = approxSpectralMoments(g, l);
    runEnd = clock();
    printf("Moments:\n");
    for (i = 0; i <= l; i++)
        printf(" %lf\n", moments[i]);
    printf("Time: %ldms\n", (runEnd - runStart) / 1000);
    return 0;
}
