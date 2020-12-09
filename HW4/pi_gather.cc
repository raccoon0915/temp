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

    // TODO: MPI init
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    long long int count = tosses / world_size;
    long long int count_last = count + tosses % world_size;
    long long int number_in_circle = 0;
    long long int sub_result = 0, *r_buffer;
    r_buffer = (long long int*)malloc(sizeof(long long int) * world_size);    
    // TODO: use MPI_Gather
    if(world_rank > 0){
       sub_result = Monte_Carlo(count, world_rank);
       //printf("sub_result:%lld from %d\n", sub_result, world_rank);
       MPI_Gather(&sub_result, 1, MPI_LONG_LONG, &r_buffer[world_rank], 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
    }
    else{
       sub_result = Monte_Carlo(count_last, world_rank);
       MPI_Gather(&sub_result, 1, MPI_LONG_LONG, &r_buffer[world_rank], 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
    }
    if (world_rank == 0)
    {
        // TODO: PI result
        for(int i = 0; i < world_size; i++)
           number_in_circle += r_buffer[i];
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
