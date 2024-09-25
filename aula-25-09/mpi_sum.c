#include <stdio.h>
#include <mpi.h>     /* For MPI functions, etc */ 

int main(void) {
   int comm_sz;               /* Número de processos    */
   int my_rank;               /* Meu rank de processo    */
   int other_rank;

   /* Inicializa o MPI */
   MPI_Init(NULL, NULL); 

   /* Obtém o número de processos */
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 

   /* Obtém meu rank entre todos os processos */
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

   if (my_rank != 0) { 
      /* Envia mensagem para o processo 0 */
      MPI_Send(&my_rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

   } else {  
      int sum = 0;
      /* Imprime minha mensagem */
      printf("Greetings from master process! I will just print the messages received.\n");
      for (int q = 1; q < comm_sz; q++) {
         /* Recebe mensagem do processo q */
         MPI_Recv(&other_rank, 1, MPI_INT, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
         
         printf("Rank passed: %d\n", other_rank);
         sum += other_rank;
      } 

      printf("Final sum: %d\n", sum);
   }

   /* Finaliza o MPI */
   MPI_Finalize(); 

   return 0;
}
