#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>
const float AVG_SCALE_FACTOR = 1.f / RAND_MAX;
long long int Monte_Carlo(long long int toss, int rank){
   double x, y, distance;
   unsigned seed = (unsigned)time(NULL) * rank;
   long long int sum = 0;
   for(int i = 0; i < toss; i++){
      x = (double)rand_r(&seed) * AVG_SCALE_FACTOR * 2 - 1;
      y = (double)rand_r(&seed) * AVG_SCALE_FACTOR * 2 - 1;
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
    long long int *result = (long long int*)malloc(sizeof(long long int) * world_size);
    int height = log(world_size) / log(2);
    // TODO: binary tree redunction
    if(world_rank % 2 == 0){
       long long int temp_even;
       temp_even = Monte_Carlo(count, world_rank);
       result[world_rank] = temp_even;
    }
    else{
       long long int temp_odd;
       temp_odd = Monte_Carlo(count, world_rank);
       MPI_Send(&temp_odd, 1, MPI_LONG_LONG, world_rank - 1, 0, MPI_COMM_WORLD);
    }
    for(int i = 1; i < height; i++){
       if(world_rank % (int)pow(2, i) == 0){/*raccoon:*/
          long long int temp;
          MPI_Recv(&temp, 1, MPI_LONG_LONG, world_rank + pow(2, i-1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          result[world_rank] += temp;
          if(i == height-1 && world_rank == (world_size / 2)){
             MPI_Send(&result[world_rank], 1, MPI_LONG_LONG, world_rank - pow(2, i), 0, MPI_COMM_WORLD);
          }
          if(world_rank % (int)pow(2, i+1) != 0)
             MPI_Send(&result[world_rank], 1, MPI_LONG_LONG, world_rank - pow(2, i), 0, MPI_COMM_WORLD);
       }
       
    }
    if (world_rank == 0)
    {
        // TODO: PI result
	if(world_size == 1)
	   number_in_circle = result[world_rank];
	else{
           long long int temp;
           MPI_Recv(&temp, 1, MPI_LONG_LONG, world_size/2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
           number_in_circle = temp + result[0];
	}
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
