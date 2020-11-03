#include <stdio.h>
#include <thread>
#include <time.h>
#include "CycleTimer.h"

typedef struct
{
    float x0, x1;
    float y0, y1;
    unsigned int width;
    unsigned int height;
    int maxIterations;
    int *output;
    int threadId;
    int numThreads;
} WorkerArgs;

extern void mandelbrotSerial(
    float x0, float y0, float x1, float y1,
    int width, int height,
    int startRow, int numRows,
    int maxIterations,
    int output[]);

//
// workerThreadStart --
//
// Thread entrypoint.
void workerThreadStart(WorkerArgs *const args)
{

    // TODO FOR PP STUDENTS: Implement the body of the worker
    // thread here. Each thread should make a call to mandelbrotSerial()
    // to compute a part of the output image.  For example, in a
    // program that uses two threads, thread 0 could compute the top
    // half of the image and thread 1 could compute the bottom half.
    double startTime = CycleTimer::currentSeconds();
    float x0 = args->x0;
    //printf("%d x0:%f\n", args->threadId, x0);
    float x1 = args->x1;
    //printf("%d x1:%f\n", args->threadId, x1);
    float y0 = args->y0;
    //printf("%d y0:%f\n", args->threadId, y0);
    float y1 = args->y1;
    //printf("%d y1:%f\n", args->threadId, y1);
    int width = args->width;
    //printf("%d width:%d\n", args->threadId, width);
    int height = args->height;
    //printf("%d height:%d\n", args->threadId, height);
    int startRow = (args->threadId)*(args->height)/(args->numThreads);
    if(args->threadId == 0 || args->threadId == args->numThreads/2)
    	startRow = startRow;
    else if(args->threadId < args->numThreads/2)
    	startRow += (args->height)/(args->numThreads)/2;
    else
    	startRow -= (args->height)/(args->numThreads)/2;
    //printf("%d startRow:%d\n", args->threadId, startRow);
    int numRows = (args->height)/(args->numThreads);
    if(args->threadId == 0 || args->threadId == args->numThreads-1)
    	numRows += numRows/2;
    else if(args->threadId == (args->numThreads/2)-1 || args->threadId == (args->numThreads/2))
    	numRows -= numRows/2;
    //printf("%d numRow:%d\n", args->threadId, numRows);
    int maxIterations = args->maxIterations;
    //printf("%d maxIteration:%d\n", args->threadId, maxIterations);
    int* output = args->output;
    //printf("%d output:%p\n", args->threadId, output);
    
/*---------------------------do------------------------------------*/    
    mandelbrotSerial(x0, y0, x1, y1, width, height, startRow, numRows, maxIterations, output);
    double endTime = CycleTimer::currentSeconds();
    printf("thread %d time:%fms\n",args->threadId, endTime - startTime);
}

//
// MandelbrotThread --
//
// Multi-threaded implementation of mandelbrot set image generation.
// Threads of execution are created by spawning std::threads.
void mandelbrotThread(
    int numThreads,
    float x0, float y0, float x1, float y1,
    int width, int height,
    int maxIterations, int output[])
{
    static constexpr int MAX_THREADS = 32;

    if (numThreads > MAX_THREADS)
    {
        fprintf(stderr, "Error: Max allowed threads is %d\n", MAX_THREADS);
        exit(1);
    }

    // Creates thread objects that do not yet represent a thread.
    std::thread workers[MAX_THREADS];
    WorkerArgs args[MAX_THREADS];

    for (int i = 0; i < numThreads; i++)
    {
        // TODO FOR PP STUDENTS: You may or may not wish to modify
        // the per-thread arguments here.  The code below copies the
        // same arguments for each thread
        args[i].x0 = x0;
        args[i].y0 = y0;
        args[i].x1 = x1;
        args[i].y1 = y1;
        args[i].width = width;
        args[i].height = height;
        args[i].maxIterations = maxIterations;
        args[i].numThreads = numThreads;
        args[i].output = output;

        args[i].threadId = i;
    }

    // Spawn the worker threads.  Note that only numThreads-1 std::threads
    // are created and the main application thread is used as a worker
    // as well.
    
    for (int i = 1; i < numThreads; i++)
    {
    	//double startTime = CycleTimer::currentSeconds();
        workers[i] = std::thread(workerThreadStart, &args[i]);
        //double endTime = CycleTimer::currentSeconds();
        //printf("thread %d time:%fms\n", i, endTime - startTime);
    }
    //double startTime = CycleTimer::currentSeconds();	
    workerThreadStart(&args[0]);
    //double endTime = CycleTimer::currentSeconds();
    //printf("thread 0 time:%fms\n", endTime - startTime);
    // join worker threads
    for (int i = 1; i < numThreads; i++)
    {
        workers[i].join();
    }
}
