#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>
void merge(long long int sub_result, int world_size, int world_rank){
   world_size /= 2;
   world_rank /= 2;
   MPI_Send(&sub_result, 1, MPI_LONG_LONG, world_rank, 0, MPI_COMM_WORLD);
   if(world_size == 2)return;
   if(world_rank < world_size){
      long long int temp_a = 0, temp_b = 0, temp_result = 0;
      MPI_Recv(&temp_a, 1, MPI_LONG_LONG, world_rank * 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(&temp_b, 1, MPI_LONG_LONG, world_rank * 2 + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      temp_result = temp_a + temp_b;
      merge(temp_result, world_size, world_rank);
   }
}
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
    
    // TODO: binary tree redunction
    if(world_rank > 0){
       long long int temp = 0;
       temp = Monte_Carlo(count, world_rank);
       merge(temp, world_size, world_rank); 
    }
    if (world_rank == 0)
    {
        // TODO: PI result
        long long int temp_a = 0, temp_b = 0, temp_result = 0;
        MPI_Recv(&temp_a, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&temp_b, 1, MPI_LONG_LONG, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        temp_result = temp_a + temp_b;
        pi_result = 4 * temp_result / ((double)tosses);
        // --- DON'T TOUCH ---
        double end_time = MPI_Wtime();
        printf("%lf\n", pi_result);
        printf("MPI running time: %lf Seconds\n", end_time - start_time);
        // ---
    }

    MPI_Finalize();
    return 0;
}
