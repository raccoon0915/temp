#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>
/*raccoon:similar to shared memory*/
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
int fnz (long long int *schedule, int size)
{
    int diff = 0;
    if (diff)
    {
       int res = 0;
       for (int i = 0; i < size; i++)
       {
          if(schedule[i] != 0)
             res++;
       }
       return(res == size-1);
    }
    return 0;
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

    MPI_Win win;

    // TODO: MPI init
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    long long int count = tosses / world_size;
    long long int count_last = count + tosses % world_size;
    long long int number_in_circle = 0;
    if (world_rank == 0)
    {
        // Master
        long long int *oldschedule = (long long int*)malloc(world_size * sizeof(long long int));
        long long int *schedule;
        MPI_Alloc_mem(world_size * sizeof(long long int), MPI_INFO_NULL, &schedule);
        for(int i = 0; i < world_size; i++)
        {
           schedule[i] = 0;
        }
        MPI_Win_create(schedule, world_size * sizeof(long long int), sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &win);
        schedule[world_rank] = Monte_Carlo(count_last, world_rank);
        int ready = 0;
        while (!ready)
        {
           MPI_Win_lock(MPI_LOCK_SHARED, 0, 0, win);
           ready = fnz(schedule, world_size);
           MPI_Win_unlock(0, win);
        }
        for(int i = 0; i < world_size; i++)
           number_in_circle += schedule[i + world_size];
        MPI_Win_free(&win);
        MPI_Free_mem(schedule);
    }
    else
    {
        // Workers
        long long int sub_result;
        sub_result = Monte_Carlo(count, world_rank);
        MPI_Win_create(NULL, 0, 1, MPI_INFO_NULL, MPI_COMM_WORLD, &win);
        MPI_Win_lock(MPI_LOCK_EXCLUSIVE, 0, 0, win);
        MPI_Put(&sub_result, 1, MPI_LONG_LONG, 0, world_rank, 1, MPI_LONG_LONG, win);
        MPI_Win_unlock(0, win);
        MPI_Win_free(&win);
    }

    if (world_rank == 0)
    {
        // TODO: handle PI result
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
