#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>


double *vetor;
long vsize;
double sum = 0.0;


int main(int argc, char *argv[])
{
  struct timeval start, stop;
  
  vsize = strtol(argv[1], NULL, 10);

  vetor = (double *)malloc(vsize*sizeof(double));

  long i;
  for (i=0; i<vsize; i++) 
    vetor[i] = (double)i;


  gettimeofday(&start, NULL);

  for (i=0; i<vsize; i++) {
    sum += vetor[i];
  }
  
  gettimeofday(&stop, NULL);


  double temp = \
    (((double)(stop.tv_sec)*1000.0 + (double)(stop.tv_usec/1000.0)) - \
    ((double)(start.tv_sec)*1000.0 + (double)(start.tv_usec/1000.0)));

  fprintf(stdout, "Time elapsed = %g ms\n", temp);
  fprintf(stdout, "Sum = %g\n", sum);


	return 0;
}

