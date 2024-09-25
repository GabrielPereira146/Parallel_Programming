#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

int main(int argc, char* argv[]) {

  srand(0);
  int i, count, n;
  double x,y,z,pi;
  struct timeval start, stop;

  if (argc < 2) {
    printf("Necessário informar um número N.\n");
    exit(-1);
  }
  n = strtoll(argv[1], NULL, 10);

  count = 0;

  gettimeofday(&start, NULL);
  for (i=0; i < n; ++i) {

    x = (double)rand() / RAND_MAX;
    y = (double)rand() / RAND_MAX;

    z = x * x + y * y;

    if( z <= 1 ) count++;
  }

  pi = (double) count / n * 4;
  gettimeofday(&stop, NULL);

  printf(" Nossa estimativa de pi = %.14f\n", pi);
  printf("                     pi = %.14f\n", 4.0*atan(1.0));
   
  double t = (((double)stop.tv_sec)*1000.0 + ((double)(stop.tv_usec)/1000.0)) - \
             (((double)start.tv_sec)*1000.0 + ((double)(start.tv_usec)/1000.0));

  fprintf(stdout, "Tempo gasto: %g ms\n", t);

  return(0);
}