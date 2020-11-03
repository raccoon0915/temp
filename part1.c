//
//  main.c
//  PP_HW2
//
//  Created by 陳弘翌 on 2020/10/27.
//  Copyright © 2020 陳弘翌. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
struct thread_data{
    int toss;
    unsigned number_in_circle;
};
void* Monte_Carlo(void* arg){
    double x, y, distance;
    unsigned sum = 0;
    int toss = ((struct thread_data*)arg)->toss;
    unsigned int seed = (unsigned)time(NULL) ^ (unsigned)pthread_self();/*raccoon:set seed*/
    for(int i=0;i<toss; i++){
        x = (double)rand_r(&seed)/RAND_MAX*2-1;
        y = (double)rand_r(&seed)/RAND_MAX*2-1;
        distance = sqrt(x*x+y*y);
        if(distance <= 1){
            sum++;
        }
    }
    ((struct thread_data*)arg)->number_in_circle = sum;
    return 0;
}
int main(int argc, const char * argv[]) {
//    int input = atoi(argv[1]);    for command line
    /*-----------input data---------------*/
    int core, toss;
    printf("core:");
    scanf("%d", &core);
    printf("toss:");
    scanf("%d", &toss);
    /*-----------PP declaration---------------*/
    struct timeval start, end;
    gettimeofday(&start, 0); /*raccoon:Time start here.*/
    unsigned number_in_circle = 0;
    int slice = 0, slice_end = 0;/*raccoon:Divide work, slice = first core-1 threads, slice_end = last one*/
    double pi;
    struct thread_data* thread_data = malloc(core*sizeof(struct thread_data));
    pthread_t *tid = malloc(core*sizeof(pthread_t));
    /*-----------PP segment---------------*/
    if(toss%core == 0){
        slice = toss/core;
        slice_end = slice;
    }
    else{
        slice = toss/core;
        slice_end = toss/core + toss%core;
    }
    for(int i=0; i<core; i++){
        thread_data[i].toss = slice;
        if(i == core-1)
            thread_data[i].toss = slice_end;
        pthread_create(&tid[i], NULL, Monte_Carlo, (void*)&thread_data[i]);
    }
    for(int i=0; i<core; i++)
        pthread_join(tid[i], NULL);/*raccoon:parent thread wait for children*/
    for(int i=0; i<core; i++)
        number_in_circle += thread_data[i].number_in_circle;
    pi = 4*number_in_circle/((double)toss);
    gettimeofday(&end, 0);
    long sec = end.tv_sec - start.tv_sec;
    long usec = end.tv_usec - start.tv_usec;
    printf("%lf\n", pi);
    printf("time:%fsec\n", (sec*1000+(usec/1000.0))/1000);
    free(tid);
    free(thread_data);
}

