#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>
const float AVG_SCALE_FACTOR = 1.f / RAND_MAX;
long long int Monte_Carlo(long long int tosses, int rank){
    double x, y, distance;
    long long int sum = 0;
    unsigned seed = (unsigned)time(NULL) * rank;
    for(int i = 0; i < tosses; i++){
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

    // TODO: init MPI
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    long long int count_last = 0;
    long long int count = tosses / world_size;
    long long int number_in_circle = 0;
    long long int *result = (long long int*)malloc(sizeof(long long int) * world_size);
    if(tosses % world_size != 0)
       count_last = count + tosses % world_size;
    else
       count_last = count;
    if (world_rank > 0)
    {
        // TODO: handle workers
	long long int temp = 0;
	temp = Monte_Carlo(count, world_rank);
        MPI_Send(&temp, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);
    }
    else if (world_rank == 0)
    {
        // TODO: master
	result[world_rank] = Monte_Carlo(count_last, world_rank);
    }

    if (world_rank == 0)
    {
        // TODO: process PI result
	for(int i = 1; i < world_size; i++)
           MPI_Recv(&result[i], 1, MPI_LONG_LONG, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	for(int i = 0; i < world_size; i++)
	   number_in_circle += result[i];
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
