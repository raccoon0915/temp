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
void matrix_multiply(const int n, const int m, const int l,
                     const int *a_mat, const int *b_mat);

// Remember to release your allocated memory
void destruct_matrices(int *a_mat, int *b_mat);
using namespace std;
int main () {
    int n, m, l;
    int *a_mat, *b_mat;
    MPI_Init(NULL, NULL);
    double start_time = MPI_Wtime();
    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    if(world_rank == 0){
       construct_matrices(&n, &m, &l, &a_mat, &b_mat);
       matrix_multiply(n, m, l, a_mat, b_mat);
       destruct_matrices(a_mat, b_mat);
       double end_time = MPI_Wtime();
       printf("MPI running time: %lf Seconds\n", end_time - start_time);
    }
    MPI_Finalize();
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
void matrix_multiply(const int n, const int m, const int l,
                     const int *a_mat, const int *b_mat){
   long long int *result = (long long int*)malloc(sizeof(long long int)*n*l);
   for(int i = 0; i < n; i++)
      for(int j = 0; j < l; j++){
	 result[j + i*l] = 0;
         for(int k = 0; k < m; k++)
            result[j + i*l] += a_mat[k + i*m] * b_mat[k*l + j];
      }
   for(int i = 0; i < n; i++)
      for(int j = 0; j < l; j++){
         cout << result[j + i*l] << " ";
         if(j % l == l - 1)
            cout << endl;
      }
}
void destruct_matrices(int *a_mat, int *b_mat){
   free(a_mat);
   free(b_mat);  
}
