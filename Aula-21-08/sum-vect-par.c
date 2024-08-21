#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

double *vetor;
long vsize;
double sum = 0.0;
float rest; 
pthread_mutex_t mutex;
int thread_count;

void *sumThread(void *id)
{
    int t_id = (int)id;
    long aux = 0;
    long t_bloco = (vsize / thread_count);
    long p_vector = t_id * t_bloco;
    if (t_id == thread_count - 1)
    {
        t_bloco += rest;
    }

    for (int i = p_vector; i < p_vector + t_bloco; i++)
    {
        aux+= vetor[i];
    }
    pthread_mutex_lock(&mutex);
    sum += aux;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(int argc, char *argv[])
{

    pthread_t *thread_handles;
    thread_count = strtol(argv[2], NULL, 10);
    thread_handles = malloc(thread_count * sizeof(pthread_t));
    struct timeval start, stop;
    pthread_mutex_init(&mutex, NULL);
    vsize = strtol(argv[1], NULL, 10);

    vetor = (double *)malloc(vsize * sizeof(double));
    rest = vsize % thread_count;
    long i;
    for (i = 0; i < vsize; i++)
        vetor[i] = (double)i;

    gettimeofday(&start, NULL);

    for (i = 0; i < thread_count; i++)
    {
        if (pthread_create(&thread_handles[i], NULL, sumThread, (void *)(i)) != 0)
        {
            fprintf(stderr, "Nao consegui criar a thread\n");
            exit(-1);
        }
    }
    /* JOIN */
    for (i = 0; i < thread_count; i++)
    {
        pthread_join(thread_handles[i], NULL);
    }

    free(thread_handles);
    pthread_mutex_destroy(&mutex);
    gettimeofday(&stop, NULL);

    double temp =
        (((double)(stop.tv_sec) * 1000.0 + (double)(stop.tv_usec / 1000.0)) -
         ((double)(start.tv_sec) * 1000.0 + (double)(start.tv_usec / 1000.0)));

    fprintf(stdout, "Time elapsed = %g ms\n", temp);
    fprintf(stdout, "Sum = %g\n", sum);

    return 0;
}
