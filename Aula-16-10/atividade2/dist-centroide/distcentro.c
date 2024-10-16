#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
 
 
int main(int argc, char **argv) 
{
  int i, j;
  struct timeval start, stop;
  
  if (argc < 2) {
    printf("Necessário informar o número de pontos.\n");
    exit(-1);
  }
  int npontos = atoi(argv[1]);

  // aloca memória para os pontos (x,y) - i-ésimo ponto está na posição i*2 e
  // i*2+1 do vetor
  int *pvetor = (int *)malloc(npontos*2*sizeof(int));

  // aloca vetor de saída para as distâncias
  double *dist = (double *)malloc(npontos*sizeof(double));

  // gera os pontos de forma aleatória
  srand(0);
  for (i=0; i<npontos; i++)
  {
    pvetor[i] = rand() % 2048;
    pvetor[i+1] = rand() % 2048;
  }
  
  gettimeofday(&start, NULL);

  // calcula a centroide
  double x = 0, y = 0;
  for (i = 0; i < npontos; i++) {
    x += pvetor[i];
    y += pvetor[i+1];
  }
  x = x / npontos;
  y = y / npontos;

  // calcula as distâncias euclidianas de cada ponto para a centróide
  for (i = 0; i < npontos; i+=2) {  
    double a = pvetor[i], b = pvetor[i+1];
    dist[i] = sqrt((x - a) * (x - a)  +  (y - b) * (y - b));
  }
  
  gettimeofday(&stop, NULL); 
  
  double t = (((double)(stop.tv_sec)*1000.0  + (double)(stop.tv_usec / 1000.0)) - \
                   ((double)(start.tv_sec)*1000.0 + (double)(start.tv_usec / 1000.0)));

  fprintf(stdout, "Tempo decorrido = %g ms\n", t);

  printf("Vetor com as distâncias para a centroide (%g,%g):\n", x, y);
  
  // imprime o vetor com as distâncias
  for (i=0; i < npontos; i++) {
    printf("%.3f ", dist[i]);
  }
  printf("\n");
 
  return 0;
}
