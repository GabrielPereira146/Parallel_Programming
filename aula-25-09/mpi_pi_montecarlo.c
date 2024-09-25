#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
  int comm_sz; /* Número de processos    */
  int my_rank; /* Meu rank de processo    */

  int i, count, n;
  double x, y, z, pi;
  struct timeval start, stop;

  if (argc < 2)
  {
    printf("Necessário informar um número N.\n");
    exit(-1);
  }
  n = strtoll(argv[1], NULL, 10);

  /* Inicializa o MPI */
  MPI_Init(&argc, &argv);

  /* Obtém o número de processos */
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

  /* Obtém meu rank entre todos os processos */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  srand(my_rank);
  count = 0;

  gettimeofday(&start, NULL);
  for (i = 0; i < n / comm_sz; ++i)
  {

    x = (double)rand() / RAND_MAX;
    y = (double)rand() / RAND_MAX;

    z = x * x + y * y;

    if (z <= 1)
      count++;
  }
  if (my_rank == 0)
  {

    /* Recebe counts dos outros nós e calcula o pi final*/
    // pi = (double) count / n * 4;
    int count_received;
    int count_total = count;
    for (int q = 1; q < comm_sz; q++)
    {
      /* Recebe count do processo q */
      MPI_Recv(&count_received, 1, MPI_INT, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      count_total += count_received;
    }
    pi = (double) count_total / n * 4;
    gettimeofday(&stop, NULL);
    printf(" Nossa estimativa de pi = %.14f\n", pi);
    printf("                     pi = %.14f\n", 4.0 * atan(1.0));

    double t = (((double)stop.tv_sec) * 1000.0 + ((double)(stop.tv_usec) / 1000.0)) -
               (((double)start.tv_sec) * 1000.0 + ((double)(start.tv_usec) / 1000.0));

    fprintf(stdout, "Tempo gasto: %g ms\n", t);
  }
  else
  {
    // Enviar os counts para o nó central
    MPI_Send(&count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }

  

  MPI_Finalize();
  return (0);
}