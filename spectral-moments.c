#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include "uthash-master/src/uthash.h"


/**
 * Generate a random number between 0 and 1, including 0 and excluding 1.
 */
double frandom()
{
    static char init = 0;
    if (!init)
    {
        // This will be done just once
        srand(time(NULL));
        init = 1;
    }
    return (double) rand() / ((long) RAND_MAX + 1);
}

/**
 * Generate a random number between start and end, including start and excluding end.
 */
double frandom_range(double start, double end)
{
    double diff = frandom() * (end - start);
    return start + diff;
}

/**
 * Generate a random number between 0 and rmax, including 0 and excluding rmax.
 */
double frandom_max(double rmax)
{
    return frandom() * rmax;
}


typedef struct struct_edge
{
    long src;
    long dst;
} t_edge;

typedef struct struct_edges_range
{
    long src;
    long offset;
    long degree;
} t_edges_range;


int compare_edges(t_edge l_edge, t_edge r_edge)
{
    if (l_edge.src > r_edge.src)
        return 1;
    if (l_edge.src < r_edge.src)
        return -1;
    if (l_edge.dst > r_edge.dst)
        return 1;
    if (l_edge.dst < r_edge.dst)
        return -1;
    return 0;
}


long binary_search_range(t_edges_range* ranges, long ranges_count, long src)
{
    if (ranges_count == 0)
        return -1;
    long l = 0;
    long r = ranges_count - 1;
    while (l <= r)
    {
        long m = l + (r - l) / 2;
        if (src == ranges[m].src)
            return m;
        if (src < ranges[m].src)
            r = m - 1;
        else
            l = m + 1;
    }
    return -1;
}


/**
 * SORT EDGES
 * 
*/

void _sort_merge_edges(t_edge *edges, long l, long m, long r)
{
    long i, j, k;
    long n1 = m - l + 1;
    long n2 =  r - m;
    /* create temp arrays */
    t_edge *L = malloc(sizeof(t_edge) * n1);
    t_edge *R = malloc(sizeof(t_edge) * n2);
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = edges[l + i];
    for (j = 0; j < n2; j++)
        R[j] = edges[m + 1 + j];
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (compare_edges(L[i], R[j]) <= 0)
        {
            edges[k] = L[i];
            i++;
        }
        else
        {
            edges[k] = R[j];
            j++;
        }
        k++;
    }
    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {
        edges[k] = L[i];
        i++;
        k++;
    }
    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        edges[k] = R[j];
        j++;
        k++;
    }
    free(R);
    free(L);
}

void _sort_edges(t_edge *edges, long l, long r)
{
    if (l < r)
    {
        long m = l + (r - l) / 2;
        _sort_edges(edges, l, m);
        _sort_edges(edges, m + 1, r);
        _sort_merge_edges(edges, l, m, r);
    }
}

void sort_edges(t_edge *edges, long edges_count)
{
    _sort_edges(edges, 0, edges_count - 1);
}


/**
 * GET ACCUMULATED STOCHASTIC MATRIX ELEMENT
 * 
 * Edges and their destitation arrays must be sorted
 * */
double getMcElement(t_edge* edges, long edges_count, t_edges_range *ranges, long ranges_count, long src, long dst)
{
    long rangeIdx = binary_search_range(ranges, ranges_count, src);
    if (rangeIdx == -1 || ranges[rangeIdx].degree == 0)
        return 0.0;
    long r = ranges[rangeIdx].degree - 1 + ranges[rangeIdx].offset;
    long l = ranges[rangeIdx].offset;
    long m;
    while (l <= r)
    {
        m = l + (r - l) / 2;
        if (dst == edges[m].dst)
            return (double) (m - ranges[rangeIdx].offset + 1) / ranges[rangeIdx].degree;
        if (dst < edges[m].dst)
            r = m - 1;
        else
            l = m + 1;
    }
    if (dst < edges[m].dst)
        m--;
    return (double) (m - ranges[rangeIdx].offset + 1) / ranges[rangeIdx].degree;
}


/**
 * CALCULATING MOMENTS
 * */

double* approxSpectralMoments(t_edge *edges, long edges_count, t_edges_range *ranges, long ranges_count, long n, long l, long s)
{
    double* x = (double*) malloc(sizeof(double) * (l+1));
    for (long i = 0; i <= l; i++)
        x[i] = 0;

    for (long i = 0; i < s; i++)
    {
        long w, j;
        w = j = (long) frandom_range(1, n+1);

        for (long k = 0; k <= l; k++)
        {
            double u = frandom_max(getMcElement(edges, edges_count, ranges, ranges_count, w, n-1));
            long w1 = 0;
            long start = 0, end = n + 1;
            while (start <= end)
            {
                long middle = start + (end - start) / 2;
                double minLim = middle > 0 ? getMcElement(edges, edges_count, ranges, ranges_count, w, middle - 1) : 0.0;
                double maxLim = getMcElement(edges, edges_count, ranges, ranges_count, w, middle);
                if (u >= minLim && u < maxLim)
                {
                    w1 = middle;
                    break;
                }
                else if (u >= minLim)
                {
                    start = middle + 1;
                }
                else
                {
                    end = middle - 1;
                }
            }
            w = w1;
            if (w == j)
                x[k] += 1.0 / s;
        }
    }
    return x;
}


#define s 20000
//#define s 1000000

int main(int argc, char** argv)
{
    FILE* file = stdin;
    clock_t runStart, runEnd;

    long n;
    fscanf(file, "%ld", &n);
    long l = argc >= 2 ? atoi(argv[1]) : 10;

    long edges_count = 0;
    t_edge *edges = NULL;
    while (1)
    {
        t_edge edge;
        if (fscanf(file, "%ld", &edge.src) == EOF)
            break;
        fscanf(file, "%ld", &edge.dst);
        if (edges != NULL)
            edges = realloc(edges, sizeof(t_edge) * ++edges_count);
        else
            edges = malloc(sizeof(t_edge) * ++edges_count);
        edges[edges_count - 1] = edge;
        // if (edges_count % 1000000 == 0)
        //     printf("%ld\n", edges_count);
    }

    sort_edges(edges, edges_count);

    long ranges_count = 0;
    t_edges_range *ranges = NULL;
    for (long i = 0; i < edges_count; i++)
    {
        // printf("%ld %ld\n", edges[i].src, edges[i].dst);
        if (i == 0)
        {
            ranges = malloc(sizeof(t_edges_range));
            ranges[0].src = edges[i].src;
            ranges[0].offset = 0;
            ranges[0].degree = 1;
            ranges_count++;
        }
        else
        {
            if (edges[i].src != edges[i-1].src)
            {
                ranges = realloc(ranges, sizeof(t_edges_range) * (ranges_count + 1));
                ranges[ranges_count].src = edges[i].src;
                ranges[ranges_count].offset = i;
                ranges[ranges_count].degree = 1;
                ranges_count++;
            }
            else
            {
                ranges[ranges_count - 1].degree++;
            }
        }
    }

    // printf("Ranges:\nsrc\toffset\tdegree\n");
    // for (int i = 0; i < ranges_count; i++)
    // {
    //     printf("%ld\t%ld\t%ld\n", ranges[i].src, ranges[i].offset, ranges[i].degree);
    // }

#if 0
     printf("Cumulative stoastic matrix:\n");
     for (long i = 1; i <= n; i++)
     {
         for (long j = 1; j <= n; j++)
         {
             printf("\t%lf", getMcElement(edges, edges_count, ranges, ranges_count, i, j));
         }
         printf("\n");
     }
#endif

  printf("Calculating...\n");
    runStart = clock();
    double* moments = approxSpectralMoments(edges, edges_count, ranges, ranges_count, n, l, s);
    runEnd = clock();
    printf("Moments:\n");
    for (long i = 0; i <= l; i++)
        printf(" %lf\n", moments[i]);
    printf("Time: %ldms\n", (runEnd - runStart) / 1000);
    return 0;
}
