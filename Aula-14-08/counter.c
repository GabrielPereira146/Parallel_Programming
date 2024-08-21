#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>

volatile unsigned long counter = 0;
unsigned long limit = 0xFFFFFFFC;

int thread_count;

void *counterThread(void *id)
{
  int my_id = (int)id;
  unsigned long i;
  volatile unsigned long aux = 0;
  for (i = my_id * (limit / thread_count); i < (my_id + 1) * (limit / thread_count); i++)
  {
    aux++;
  }

  return (void *)aux;
}

int main(int argc, char *argv[])
{
  pthread_t *thread_handles;
  thread_count = strtol(argv[1], NULL, 10);
  thread_handles = malloc(thread_count * sizeof(pthread_t));

  struct timeval start, stop;
  int i;
  gettimeofday(&start, NULL);
  
  for (i = 0; i < thread_count; i++)
  {
    if (pthread_create(&thread_handles[i], NULL, counterThread, (void *)(i)) != 0)
    {
      fprintf(stderr, "Nao consegui criar a thread\n");
      exit(-1);
    }
  }
  /* JOIN */
  // unsigned long ret;
  void* ret;
  for (i = 0; i < thread_count; i++)
  {
    pthread_join(thread_handles[i], ret);
    fprintf(stderr,"PASSOU\n");
    counter+= (unsigned long)ret;
  }

  free(thread_handles);

  gettimeofday(&stop, NULL);

  printf("counter = %x\n", counter);
  double temp =
      (((double)(stop.tv_sec) * 1000.0 + (double)(stop.tv_usec / 1000.0)) -
       ((double)(start.tv_sec) * 1000.0 + (double)(start.tv_usec / 1000.0)));
  printf("Time elapsed = %g ms\n", temp);

  return 0;
}
