/* WSCAD - 9th Marathon of Parallel Programming
 * Simple Brute Force Algorithm for the
 * Traveling-Salesman Problem
 * Author: Emilio Francesquini - francesquini@ic.unicamp.br
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <sys/time.h>

int min_distance;
int nb_towns;

typedef struct
{
    int to_town;
    int dist;
} d_info;

d_info **d_matrix;
int *dist_to_origin;

/**
 * Checks if a given town is present in the current path up to a specified depth.
 *
 * @param town The town to check for presence in the path.
 * @param depth The current depth of the path being explored.
 * @param path An array representing the sequence of towns visited so far.
 * @return 1 if the town is present in the path, otherwise 0.
 */
int present(int town, int depth, int *path)
{
    int i;
    for (i = 0; i < depth; i++)
        if (path[i] == town)
            return 1;
    return 0;
}

/**
 * Recursively explore all possible routes for the TSP.
 *
 * The tsp function is a recursive function that explores all possible routes
 * for the TSP. It takes as input the current depth of the path being explored,
 * the length of the path so far, and an array of length nb_towns to store
 * the sequence of towns visited so far.
 *
 * @param depth The current depth of the path being explored.
 * @param current_length The length of the path so far.
 * @param path An array of length nb_towns to store the sequence of towns
 *             visited so far.
 */
void tsp(int depth, int current_length, int *path)
{
    int i;
    if (current_length >= min_distance)
        return;
    if (depth == nb_towns)
    {
        current_length += dist_to_origin[path[nb_towns - 1]];
        if (current_length < min_distance)
            min_distance = current_length;
    }
    else
    {
        int town, me, dist;
        me = path[depth - 1];
        for (i = 0; i < nb_towns; i++)
        {
            town = d_matrix[me][i].to_town;
            if (!present(town, depth, path))
            {
                path[depth] = town;
                dist = d_matrix[me][i].dist;
#pragma omp task shared(min_distance)
                tsp(depth + 1, current_length + dist, path);
            }
        }
    }
}

/**
 * Heuristically initializes the distance matrix with the closest neighbor
 * to each town.
 *
 * This function uses a greedy algorithm to find the closest neighbor to
 * each town and store it in the distance matrix. The result is a good
 * starting point for the TSP, but may not result in the best solution.
 *
 * @param x The x coordinates of each town.
 * @param y The y coordinates of each town.
 */
void greedy_shortest_first_heuristic(int *x, int *y)
{
    int i, j, k, dist;
    int *tempdist;

    tempdist = (int *)malloc(sizeof(int) * nb_towns);
// Could be faster, albeit not as didactic.
// Anyway, for tractable sizes of the problem it
// runs almost instantaneously.

    for (i = 0; i < nb_towns; i++)
    {
        for (j = 0; j < nb_towns; j++)
        {
            int dx = x[i] - x[j];
            int dy = y[i] - y[j];
            tempdist[j] = dx * dx + dy * dy;
        }
#pragma omp for
        for (j = 0; j < nb_towns; j++)
        {
            int tmp = INT_MAX;
            int town = 0;
            for (k = 0; k < nb_towns; k++)
            {
                if (tempdist[k] < tmp)
                {
                    tmp = tempdist[k];
                    town = k;
                }
            }
            tempdist[town] = INT_MAX;
            d_matrix[i][j].to_town = town;
            dist = (int)sqrt(tmp);
            d_matrix[i][j].dist = dist;
            if (i == 0)
                dist_to_origin[town] = dist;
        }
    }

    free(tempdist);
}

/**
 * Initialize the TSP problem.
 *
 * Reads the number of towns and the coordinates of each town from standard
 * input, then allocates memory for the distance matrix and the vector of
 * distances to the origin, and initializes them using the greedy shortest
 * first heuristic.
 *
 * @return void
 */
void init_tsp()
{
    int i, st;
    int *x, *y;

    min_distance = INT_MAX;
    printf("Numero de Cidades: ");
    st = scanf("%u", &nb_towns);
    if (st != 1)
        exit(1);

    d_matrix = (d_info **)malloc(sizeof(d_info *) * nb_towns);
    for (i = 0; i < nb_towns; i++)
        d_matrix[i] = (d_info *)malloc(sizeof(d_info) * nb_towns);
    dist_to_origin = (int *)malloc(sizeof(int) * nb_towns);

    x = (int *)malloc(sizeof(int) * nb_towns);
    y = (int *)malloc(sizeof(int) * nb_towns);

    printf("Coordenadas das cidades:\n");
    for (i = 0; i < nb_towns; i++)
    {
        printf("Cidade %d: ", i);
        st = scanf("%u %u", x + i, y + i);
        if (st != 2)
            exit(1);
    }

    greedy_shortest_first_heuristic(x, y);

    free(x);
    free(y);
}

/**
 * Solve the TSP using the greedy shortest first heuristic.
 *
 * This function initializes the TSP problem using the greedy shortest
 * first heuristic, and then uses a recursive function to explore all
 * possible routes for the TSP. It returns the minimum distance of the
 * routes found.
 *
 * @return The minimum distance of the routes found.
 */
int run_tsp()
{
    int i, *path;
    struct timeval start, stop;
    init_tsp();
    gettimeofday(&start, NULL);
    path = (int *)malloc(sizeof(int) * nb_towns);
    path[0] = 0;
#pragma omp single
    tsp(1, 0, path);

    free(path);
    for (i = 0; i < nb_towns; i++)
        free(d_matrix[i]);
    free(d_matrix);
    gettimeofday(&stop, NULL);
    double t = (((double)(stop.tv_sec) * 1000.0 + (double)(stop.tv_usec / 1000.0)) -
                ((double)(start.tv_sec) * 1000.0 + (double)(start.tv_usec / 1000.0)));

    fprintf(stdout, "Tempo decorrido = %g ms\n", t);
    return min_distance;
}

/**
 * Main entry point of the TSP solver.
 *
 * This function reads the number of TSP instances from the standard input,
 * and then for each instance, it reads the number of towns and the
 * coordinates of the towns, and prints the minimum distance of a route
 * that visits all towns exactly once and returns to the origin.
 *
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 * @return 0 if successful, -1 otherwise.
 */
int main(int argc, char **argv)
{

#pragma omp parallel
    {

        int num_instances, st;
        printf("Vezes que o problema deve ser resolvido: ");
        st = scanf("%u", &num_instances);
        if (st != 1)
            exit(1);
        while (num_instances-- > 0)
            printf("%d\n", run_tsp());
        return 0;
    }
}