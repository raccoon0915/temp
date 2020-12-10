#include <mpi.h>
#include <cstdio>
#include <iostream>
// *********************************************
// ** ATTENTION: YOU CANNOT MODIFY THIS FILE. **
// *********************************************

// Read size of matrix_a and matrix_b (n, m, l) and whole data of matrixes from stdin
//
// n_ptr:     pointer to n
// m_ptr:     pointer to m
// l_ptr:     pointer to l
// a_mat_ptr: pointer to matrix a (a should be a continuous memory space for placing n * m elements of int)
// b_mat_ptr: pointer to matrix b (b should be a continuous memory space for placing m * l elements of int)
void construct_matrices(int *n_ptr, int *m_ptr, int *l_ptr,
                        int **a_mat_ptr, int **b_mat_ptr);

// Just matrix multiplication (your should output the result in this function)
// 
// n:     row number of matrix a
// m:     col number of matrix a / row number of matrix b
// l:     col number of matrix b
// a_mat: a continuous memory placing n * m elements of int
// b_mat: a continuous memory placing m * l elements of int
int *matrix_multiply(const int n, const int m, const int l, int count,
                     const int *a_mat, const int *b_mat);

// Remember to release your allocated memory
void destruct_matrices(int *a_mat, int *b_mat, int *c_mat);
using namespace std;
int main () {
    int n, m, l;
    int *a_mat, *b_mat, *c_mat;
    c_mat = (int*)malloc(sizeof(int)*2000*2000);
    MPI_Init(NULL, NULL);
    double start_time = MPI_Wtime();
    int world_rank, world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int count = n / world_size;
    int count_last = count + n % world_size;
    if(world_rank == 0){
       construct_matrices(&n, &m, &l, &a_mat, &b_mat);
       int *temp;
       temp = matrix_multiply(world_rank*count, m, l, count, a_mat, b_mat);
       for(int i = 0; i < count; i++)
          c_mat[i] = temp[i];
    }
    else if(world_rank != world_size - 1){
       int *temp;
       temp = matrix_multiply(world_rank * count, m, l, count, a_mat, b_mat);
       MPI_Send(temp, count, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    else{
       int *temp;
       temp = matrix_multiply(world_rank * count_last, m, l, count_last, a_mat, b_mat);
       MPI_Send(temp, count_last, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    
    if(world_rank == 0){
       for(int i = 1; i < world_size; i++){
          if(i == world_size - 1)
             MPI_Recv(&c_mat[i], count_last, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          else
             MPI_Recv(&c_mat[i], count, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
       }
       for(int i = 0; i < n; i++)
          for(int j = 0; j < l; j++){
             cout << c_mat[j + i*l] << " ";
             if(j % l == l - 1)
                cout << endl;
          }
    }
    
    destruct_matrices(a_mat, b_mat, c_mat);
    
    double end_time = MPI_Wtime();
    MPI_Finalize();
    printf("MPI running time: %lf Seconds\n", end_time - start_time);

    return 0;
}
void construct_matrices(int *n_ptr, int *m_ptr, int *l_ptr,
                        int **a_mat_ptr, int **b_mat_ptr){
   cin >> *n_ptr >> *m_ptr >> *l_ptr;
   int n = *n_ptr, m = *m_ptr, l = *l_ptr;
   *a_mat_ptr = (int*)malloc(sizeof(int)*n*m);
   *b_mat_ptr = (int*)malloc(sizeof(int)*m*l);
   for(int i = 0; i < n; i++)
      for(int j = 0; j < m; j++){
         int temp;
         cin >> temp;
         *(*a_mat_ptr + j + i*m) = temp;
      }
   for(int i = 0; i < m; i++)
      for(int j = 0; j < l; j++){
         int temp;
         cin >> temp;
         *(*b_mat_ptr + j +i*l) = temp;
      }                 
}
/*void matrix_multiply(const int n, const int m, const int l,
                     const int *a_mat, const int *b_mat, const int){
   int *result = (int*)malloc(sizeof(int)*n*l);
   for(int i = 0; i < n; i++)
      for(int j = 0; j < l; j++)
         for(int k = 0; k < m; k++){
            result[j + i*l] += a_mat[k + i*m] * b_mat[k*l + j];
         }
   for(int i = 0; i < n; i++)
      for(int j = 0; j < l; j++){
         cout << result[j + i*l] << " ";
         if(j % l == l - 1)
            cout << endl;
      }
}*/
int *matrix_multiply(const int n, const int m, const int l, int count,
                     const int *a_mat, const int *b_mat){
   int *result = (int*)malloc(sizeof(int) * count * l);
   for(int i = n; i < n + count - 1; i++)
      for(int j = 0; j < l; j++)
         for(int k = 0; k < m; k++){
            result[j + i*l] += a_mat[k + i*m] * b_mat[k*l + j];
         }
   return result;
}
void destruct_matrices(int *a_mat, int *b_mat, int *c_mat){
   free(a_mat);
   free(b_mat);
   free(c_mat);   
}
