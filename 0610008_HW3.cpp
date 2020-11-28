/*
 Student No.: 0610008
 Student Name: Yen-Chen Chang
 Email: changyenchen.eecs06@nctu.edu.tw
 SE tag: xnxcxtxuxoxsx
 Statement: I am fully aware that this program is not supposed to be posted to a public server, such as a public GitHub repository or a public web page.
 */

#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
#include <sys/types.h>
#include <limits.h>
#include <fcntl.h>

using namespace std;

sem_t *sem1;
sem_t *sem[15];
int INT_NUM = 0;

void BubbleSort(int *arr, int start, int end){
    for (int i = end; i > start; i--) {
        for (int j = start; j < i; j++) {
            if (*(arr+j) > *(arr+j+1)) {
                swap(*(arr+j), *(arr+j+1));
            }
        }
    }
}

void Merge(int *arr, int start, int mid, int end){
    int left_arr[mid-start+1];
    int right_arr[end-mid+2];
    int left_i = 0, right_i = 0;
    
    for (int i = start; i < mid; i++) {
        left_arr[left_i++] = *(arr+i);
    }
    left_arr[left_i] = INT_MAX;
    
    for (int i = mid; i <= end; i++) {
        right_arr[right_i++] = *(arr+i);
    }
    right_arr[right_i] = INT_MAX;
    
    left_i = 0;
    right_i = 0;
    
    for (int i = start; i <= end; i++) {
        if (left_arr[left_i] <= right_arr[right_i]) {
            *(arr+i) = left_arr[left_i++];
        }else{
            *(arr+i) = right_arr[right_i++];
        }
    }
}

void MergeSort(int *arr, int start, int end, int level){
    if (start < end) {
        int mid = (start+end)/2;
        if (level == 3) {
            BubbleSort(arr, start, mid);
            BubbleSort(arr, mid+1, end);
        }else{
            MergeSort(arr, start, mid, level+1);
            MergeSort(arr, mid+1, end, level+1);
        }
        Merge(arr, start, mid+1, end);
    }
}

void* STsort(int *arr){
    MergeSort(arr, 0, INT_NUM-1, 1);
    sem_post(sem1);
    pthread_exit(NULL);
}

void* MTsort0(int *arr){
    sem_post(sem[1]);
    sem_post(sem[2]);
    sem_wait(sem[0]);
    sem_wait(sem[0]);

    Merge(arr, 0, (INT_NUM+1)/2, INT_NUM-1);
    sem_post(sem1);
    pthread_exit(NULL);
}
void* MTsort1(int *arr){
    sem_wait(sem[1]);

    sem_post(sem[3]);
    sem_post(sem[4]);
    
    int start = 0;
    int end = INT_NUM-1;
    int mid = (start+end)/2;
    end = mid;
    mid = (start+end)/2;

    sem_wait(sem[1]);
    sem_wait(sem[1]);

    Merge(arr, start, mid+1, end);
    sem_post(sem[0]);
    pthread_exit(NULL);
}
void* MTsort2(int *arr){
    sem_wait(sem[2]);

    sem_post(sem[5]);
    sem_post(sem[6]);
    
    int start = 0;
    int end = INT_NUM-1;
    int mid = (start+end)/2;
    start = mid+1;
    mid = (start+end)/2;
    
    sem_wait(sem[2]);
    sem_wait(sem[2]);

    Merge(arr, start, mid+1, end);
    sem_post(sem[0]);
    pthread_exit(NULL);
}
void* MTsort3(int *arr){
    sem_wait(sem[3]);

    sem_post(sem[7]);
    sem_post(sem[8]);
    
    int start = 0;
    int end = INT_NUM-1;
    int mid = (start+end)/2;
    end = mid;
    mid = (start+end)/2;
    end = mid;
    mid = (start+end)/2;

    sem_wait(sem[3]);
    sem_wait(sem[3]);

    Merge(arr, start, mid+1, end);
    sem_post(sem[1]);
    pthread_exit(NULL);
}
void* MTsort4(int *arr){
    sem_wait(sem[4]);

    sem_post(sem[9]);
    sem_post(sem[10]);
    
    int start = 0;
    int end = INT_NUM-1;
    int mid = (start+end)/2;
    end = mid;
    mid = (start+end)/2;
    start = mid+1;
    mid = (start+end)/2;
    
    sem_wait(sem[4]);
    sem_wait(sem[4]);

    Merge(arr, start, mid+1, end);
    sem_post(sem[1]);
    pthread_exit(NULL);
}
void* MTsort5(int *arr){
    sem_wait(sem[5]);

    sem_post(sem[11]);
    sem_post(sem[12]);
    
    int start = 0;
    int end = INT_NUM-1;
    int mid = (start+end)/2;
    start = mid+1;
    mid = (start+end)/2;
    end = mid;
    mid = (start+end)/2;
    
    sem_wait(sem[5]);
    sem_wait(sem[5]);

    Merge(arr, start, mid+1, end);
    sem_post(sem[2]);
    pthread_exit(NULL);
}
void* MTsort6(int *arr){
    sem_wait(sem[6]);

    sem_post(sem[13]);
    sem_post(sem[14]);
    
    int start = 0;
    int end = INT_NUM-1;
    int mid = (start+end)/2;
    start = mid+1;
    mid = (start+end)/2;
    start = mid+1;
    mid = (start+end)/2;
    
    sem_wait(sem[6]);
    sem_wait(sem[6]);

    Merge(arr, start, mid+1, end);
    sem_post(sem[2]);
    pthread_exit(NULL);
}
void* MTsort7(int *arr){
    sem_wait(sem[7]);

    int start = 0;
    int end = INT_NUM-1;
    int mid = (start+end)/2;
    end = mid;
    mid = (start+end)/2;
    end = mid;
    mid = (start+end)/2;
    
    BubbleSort(arr, start, mid);
    sem_post(sem[3]);
    pthread_exit(NULL);
}
void* MTsort8(int *arr){
    sem_wait(sem[8]);

    int start = 0;
    int end = INT_NUM-1;
    int mid = (start+end)/2;
    end = mid;
    mid = (start+end)/2;
    end = mid;
    mid = (start+end)/2;
    BubbleSort(arr, mid+1, end);
    sem_post(sem[3]);
    pthread_exit(NULL);
}
void* MTsort9(int *arr){
    sem_wait(sem[9]);

    int start = 0;
    int end = INT_NUM-1;
    int mid = (start+end)/2;
    end = mid;
    mid = (start+end)/2;
    start = mid+1;
    mid = (start+end)/2;
    
    BubbleSort(arr, start, mid);
    sem_post(sem[4]);
    pthread_exit(NULL);
}
void* MTsort10(int *arr){
    sem_wait(sem[10]);

    int start = 0;
    int end = INT_NUM-1;
    int mid = (start+end)/2;
    end = mid;
    mid = (start+end)/2;
    start = mid+1;
    mid = (start+end)/2;
    
    BubbleSort(arr, mid+1, end);
    sem_post(sem[4]);
    pthread_exit(NULL);
}
void* MTsort11(int *arr){
    sem_wait(sem[11]);

    int start = 0;
    int end = INT_NUM-1;
    int mid = (start+end)/2;
    start = mid+1;
    mid = (start+end)/2;
    end = mid;
    mid = (start+end)/2;
    
    BubbleSort(arr, start, mid);
    sem_post(sem[5]);
    pthread_exit(NULL);
}
void* MTsort12(int *arr){
    sem_wait(sem[12]);

    int start = 0;
    int end = INT_NUM-1;
    int mid = (start+end)/2;
    start = mid+1;
    mid = (start+end)/2;
    end = mid;
    mid = (start+end)/2;
    
    BubbleSort(arr, mid+1, end);
    sem_post(sem[5]);
    pthread_exit(NULL);
}
void* MTsort13(int *arr){
    sem_wait(sem[13]);

    int start = 0;
    int end = INT_NUM-1;
    int mid = (start+end)/2;
    start = mid+1;
    mid = (start+end)/2;
    start = mid+1;
    mid = (start+end)/2;
    
    BubbleSort(arr, start, mid);
    sem_post(sem[6]);
    pthread_exit(NULL);
}
void* MTsort14(int *arr){
    sem_wait(sem[14]);

    int start = 0;
    int end = INT_NUM-1;
    int mid = (start+end)/2;
    start = mid+1;
    mid = (start+end)/2;
    start = mid+1;
    mid = (start+end)/2;
    
    BubbleSort(arr, mid+1, end);
    sem_post(sem[6]);
    pthread_exit(NULL);
}

int main(int argc, const char * argv[]) {
    
    string ifname;
    cout << "Please type in input file name: ";
    cin >> ifname;
    
    ifstream input (ifname);
    if (input.is_open()) {
        input >> INT_NUM;
    }
    
    int *arr = (int *)malloc(INT_NUM * sizeof(int));
    int i = 0;
    while (input >> arr[i++])
        ;
    input.close();

    pthread_t t1;
    pthread_t t[15];
    struct timeval start, end;
    long sec;
    int usec;

    //MULTI THREAD OPERATION START
    sem1 = sem_open("t1_semaphore", O_CREAT, 0644, 0);
    for (int i = 0; i < 15; i++) {
        char name[16] ="t[00]_semaphore";
        name[2] = '0' + (i/10);
        name[3] = '0' + (i%10);
        sem[i] = sem_open(name, O_CREAT, 0644, 0);
    }
    gettimeofday(&start, 0);
    pthread_create(&t[0], NULL, (void *(*)(void *))MTsort0, arr);
    pthread_create(&t[1], NULL, (void *(*)(void *))MTsort1, arr);
    pthread_create(&t[2], NULL, (void *(*)(void *))MTsort2, arr);
    pthread_create(&t[3], NULL, (void *(*)(void *))MTsort3, arr);
    pthread_create(&t[4], NULL, (void *(*)(void *))MTsort4, arr);
    pthread_create(&t[5], NULL, (void *(*)(void *))MTsort5, arr);
    pthread_create(&t[6], NULL, (void *(*)(void *))MTsort6, arr);
    pthread_create(&t[7], NULL, (void *(*)(void *))MTsort7, arr);
    pthread_create(&t[8], NULL, (void *(*)(void *))MTsort8, arr);
    pthread_create(&t[9], NULL, (void *(*)(void *))MTsort9, arr);
    pthread_create(&t[10], NULL, (void *(*)(void *))MTsort10, arr);
    pthread_create(&t[11], NULL, (void *(*)(void *))MTsort11, arr);
    pthread_create(&t[12], NULL, (void *(*)(void *))MTsort12, arr);
    pthread_create(&t[13], NULL, (void *(*)(void *))MTsort13, arr);
    pthread_create(&t[14], NULL, (void *(*)(void *))MTsort14, arr);

    sem_wait(sem1);
    gettimeofday(&end, 0);
    sec = end.tv_sec - start.tv_sec;
    usec = end.tv_usec - start.tv_usec;
    
    ofstream outM;
    outM.open("output1.txt");
    for (int i = 0; i < INT_NUM; i++) {
        outM << *(arr+i) << " ";
    }
    outM.close();
    
    cout << "Multi Thread Execution Time: " << sec*1.0 + usec/1000000.0 << " sec" << endl;
    
    for (int i = 0; i < 15; i++) {
        char name[16] ="t[00]_semaphore";
        name[2] = '0' + (i/10);
        name[3] = '0' + (i%10);
        sem_close(sem[i]);
        sem_unlink(name);
    }
    
    //MULTI THREAD OPERATION END
    
    //SINGLE THREAD OPERATION START
    input.open(ifname);
    input >> INT_NUM;
    i = 0;
    while (input >> arr[i++])
        ;
    input.close();
    gettimeofday(&start, 0);
    pthread_create(&t1, NULL, (void *(*)(void *))STsort, arr);
    sem_wait(sem1);
    
    gettimeofday(&end, 0);
    sec = end.tv_sec - start.tv_sec;
    usec = end.tv_usec - start.tv_usec;
    
    ofstream outS;
    outS.open("output2.txt");
    for (int i = 0; i < INT_NUM; i++) {
        outS << *(arr+i) << " ";
    }
    outS.close();
    
    cout << "Single Thread Execution Time: " << sec*1.0 + usec/1000000.0 << " sec" << endl;

    sem_close(sem1);
    sem_unlink("t1_semaphore");
    //SINGLE THREAD OPERATION END

    free(arr);

    return 0;
}
