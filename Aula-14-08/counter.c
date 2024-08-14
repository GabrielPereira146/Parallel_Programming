#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>

volatile unsigned long counter = 0;

int thread_count; 

void *counterThread(void *id)
{
  int my_id = (int)id;

  fprintf(stdout, "Hello from thread %d (total of %d threads)\n");

  return NULL;
}

int main(int argc, char *argv[])
{
  pthread_t *thread_handles;
  thread_count = strtol(argv[1], NULL, 10);
  thread_handles = malloc(thread_count * sizeof(pthread_t));

  struct timeval start, stop;

  unsigned long limit = 0xFFFFFFFC;

  unsigned long i;

  gettimeofday(&start, NULL);

  for (i = 0; i < limit; i++)
    counter++;

  gettimeofday(&stop, NULL);

  printf("counter = %x\n", counter);
  double temp =
      (((double)(stop.tv_sec) * 1000.0 + (double)(stop.tv_usec / 1000.0)) -
       ((double)(start.tv_sec) * 1000.0 + (double)(start.tv_usec / 1000.0)));
  printf("Time elapsed = %g ms\n", temp);

  return 0;
}
