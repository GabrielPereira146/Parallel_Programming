#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define SIZE 10000
int thread_count;

long int M[SIZE][SIZE];
long int v1[SIZE], v2[SIZE];

void *multiMatrixThread(void *id)
{
  int my_id = (int)id;

  for (int i = my_id * (SIZE/thread_count); i < (my_id + 1) * (SIZE/thread_count); i++)
  {
    v2[i] = 0;
    for (int j = 0; j < SIZE; j++)
    {
      v2[i] += M[i][j] * v1[j];
    }
  }

  return NULL;
}

int main(int argc, char *argv[])
{
  pthread_t *thread_handles;
  thread_count = strtol(argv[1], NULL, 10);
  thread_handles = malloc(thread_count * sizeof(pthread_t));

  struct timeval start, stop;

  int i, j;
  for (i = 0; i < SIZE; i++)
  {
    v1[i] = i;
    for (j = 0; j < SIZE; j++)
      M[i][j] = i + j;
  }

  gettimeofday(&start, NULL);

  for (i = 0; i < thread_count; i++)
  {
    if (pthread_create(&thread_handles[i], NULL, multiMatrixThread, (void *)(i)) != 0)
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

  gettimeofday(&stop, NULL);

  double temp =
      (((double)(stop.tv_sec) * 1000.0 + (double)(stop.tv_usec / 1000.0)) -
       ((double)(start.tv_sec) * 1000.0 + (double)(start.tv_usec / 1000.0)));

  fprintf(stdout, "Time elapsed = %g ms\n", temp);

  for (i = 0; i < SIZE; i++)
    fprintf(stdout, "v[%d]=%ld\n", i, v2[i]);

  return 0;
}
