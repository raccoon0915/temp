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
    long long int number_in_circle;
    long long int count = tosses / world_size;
    long long int count_last = count + tosses % world_size;
    long long int sub_result;
    // TODO: use MPI_Reduce
    if(world_rank > 0){
       sub_result = Monte_Carlo(count, world_rank);
       MPI_Reduce(&sub_result, &number_in_circle, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    }
    else{
       sub_result = Monte_Carlo(count_last, world_rank);
       MPI_Reduce(&sub_result, &number_in_circle, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    }
    if (world_rank == 0)
    {
        // TODO: PI result
        pi_result = 4 * number_in_circle/((double)tosses);
        // --- DON'T TOUCH ---
        double end_time = MPI_Wtime();
        printf("%lf\n", pi_result);
        printf("MPI running time: %lf Seconds\n", end_time - start_time);
        // ---
    }

    MPI_Finalize();
    return 0;
}
