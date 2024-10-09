#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <mpi.h>

int main(int argc, char **argv)
{
  int comm_sz; /* Número de processos    */
  int my_rank; /* Meu rank de processo    */
  int i, j;
  double centroid_x, centroid_y;

  struct timeval start, stop;

  if (argc < 2)
  {
    printf("Necessário informar o número de pontos.\n");
    exit(-1);
  }
  int npontos = atoi(argv[1]);

  /* Inicializa o MPI */
  MPI_Init(&argc, &argv);

  /* Obtém o número de processos */
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

  /* Obtém meu rank entre todos os processos */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  int *pvetor;
  double *dist;
  if (my_rank == 0)
  {
    // aloca memória para os pontos (x,y) - i-ésimo ponto está na posição i*2 e
    // i*2+1 do vetor
    pvetor = (int *)malloc(npontos * 2 * sizeof(int));

    // aloca vetor de saída para as distâncias
    dist = (double *)malloc(npontos * sizeof(double));
    // gera os pontos de forma aleatória
    srand(0);
    for (i = 0; i < npontos; i++)
    {
      pvetor[i * 2] = rand() % 2048;
      pvetor[(i * 2) + 1] = rand() % 2048;
    }
  }
  else
  {
    // aloca memória para os pontos (x,y) - i-ésimo ponto está na posição i*2 e
    // i*2+1 do vetor
    pvetor = (int *)malloc(npontos * 2 * sizeof(int) / comm_sz);

    // aloca vetor de saída para as distâncias
    dist = (double *)malloc(npontos * sizeof(double) / comm_sz);
  }

  gettimeofday(&start, NULL);
  // calcula a centroide
  MPI_Scatter(pvetor, npontos * 2 / comm_sz, MPI_INT, pvetor, npontos * 2 / comm_sz, MPI_INT, 0, MPI_COMM_WORLD);

  double x = 0, y = 0;
  for (i = 0; i < (npontos / comm_sz); i++)
  {
    x += pvetor[i * 2];
    y += pvetor[(i * 2) + 1];
  }
  MPI_Allreduce(&x, &centroid_x, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
  MPI_Allreduce(&y, &centroid_y, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

  centroid_x = centroid_x / npontos;
  centroid_y = centroid_y / npontos;

  // calcula as distâncias euclidianas de cada ponto para a centróide

  for (i = 0; i < (npontos / comm_sz); i++)
  {
    double a = pvetor[i * 2], b = pvetor[(i * 2) + 1];
    dist[i] = sqrt((centroid_x - a) * (centroid_x - a) + (centroid_y - b) * (centroid_y - b));
  }
 
  MPI_Gather(dist, npontos / comm_sz, MPI_DOUBLE,
             dist, npontos / comm_sz, MPI_DOUBLE,
             0, MPI_COMM_WORLD);

  gettimeofday(&stop, NULL);

  double t = (((double)(stop.tv_sec) * 1000.0 + (double)(stop.tv_usec / 1000.0)) -
              ((double)(start.tv_sec) * 1000.0 + (double)(start.tv_usec / 1000.0)));

  fprintf(stdout, "Tempo decorrido = %g ms\n", t);

  printf("Vetor com as distâncias para a centroide (%g,%g):\n", x, y);

  // imprime o vetor com as distâncias
  for (i = 0; i < npontos; i++)
  {
    printf("%.3f ", dist[i]);
  }
  printf("\n");
  MPI_Finalize();
  return 0;
}