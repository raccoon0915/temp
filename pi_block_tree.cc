#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>
long long int Monte_Carlo(long long int toss, int rank){
   double x, y, distance;
   unsigned seed = (unsigned)time(NULL) * rank;
   long long int sum = 0;
   for(int i = 0; i < toss; i++){
      x = (double)rand_r(&seed) / RAND_MAX * 2 - 1;
      y = (double)rand_r(&seed) / RAND_MAX * 2 - 1;
      distance = sqrt(x * x + y * y);
      if(distance <= 1)
         sum++;
   }
   return sum;
}
int main(int argc, char **argv)
{
    // --- DON'T TOUCH ---
    MPI_Init(&argc, &argv);
    double start_time = MPI_Wtime();
    double pi_result;
    long long int tosses = atoi(argv[1]);
    int world_rank, world_size;
    // ---

    // TODO: MPI init
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    long long int count = tosses / world_size;
    long long int number_in_circle = 0;
    long long int *result = (long long int*)malloc(sizeof(long long int) * world_size / 2);/*raccoon:Only require processes/2 elements to keep sub-result*/
    // TODO: binary tree redunction
    if(world_rank > 0){
       long long int temp = 0;
       temp = Monte_Carlo(count, world_rank);
       MPI_Send(&temp, 1, MPI_LONG_LONG, world_rank / 2, 0, MPI_COMM_WORLD);/*raccoon:send sub-result to world_rank/2*/
       if(world_rank < (world_size / 2)){/*raccoon:level-3 receiver*/
          long long int temp_1_a = 0, temp_1_b = 0;
          MPI_Recv(&temp_1_a, 1, MPI_LONG_LONG, world_rank * 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          MPI_Recv(&temp_1_b, 1, MPI_LONG_LONG, world_rank * 2 + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          result[world_rank] = temp_1_a + temp_1_b;
          MPI_Send(&result[world_rank], 1, MPI_LONG_LONG, world_rank / 2, 0, MPI_COMM_WORLD);
       }
       if(world_rank < (world_size / 4)){
          long long int temp_2_a = 0, temp_2_b = 0;
          MPI_Recv(&temp_2_a, 1, MPI_LONG_LONG, world_rank * 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          MPI_Recv(&temp_2_b, 1, MPI_LONG_LONG, world_rank * 2 + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          result[world_rank] = temp_2_a + temp_2_b;
          MPI_Send(&result[world_rank], 1, MPI_LONG_LONG, world_rank / 2, 0, MPI_COMM_WORLD);
       }
       if(world_rank < (world_size / 8)){
          long long int temp_3_a = 0, temp_3_b = 0;
          MPI_Recv(&temp_3_a, 1, MPI_LONG_LONG, world_rank * 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          MPI_Recv(&temp_3_b, 1, MPI_LONG_LONG, world_rank * 2 + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          result[world_rank] = temp_3_a + temp_3_b;
          MPI_Send(&result[world_rank], 1, MPI_LONG_LONG, world_rank / 2, 0, MPI_COMM_WORLD);
       }
    }
    if (world_rank == 0)
    {
        // TODO: PI result
        long long int temp_4_a = 0, temp_4_b = 0;
        MPI_Recv(&temp_4_a, 1, MPI_LONG_LONG, world_rank * 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&temp_4_b, 1, MPI_LONG_LONG, world_rank * 2 + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        number_in_circle = temp_4_a + temp_4_b;
        pi_result = 4 * number_in_circle / ((double)tosses);
        // --- DON'T TOUCH ---
        double end_time = MPI_Wtime();
        printf("%lf\n", pi_result);
        printf("MPI running time: %lf Seconds\n", end_time - start_time);
        // ---
    }

    MPI_Finalize();
    return 0;
}
