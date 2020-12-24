#include <cuda.h>
#include <stdio.h>
#include <stdlib.h>

__global__ void mandelKernel(float lowerX, float lowerY, float stepX, float stepY, int maxIterations, int* result) {
    // To avoid error caused by the floating number, use the following pseudo code
    //
    // float x = lowerX + thisX * stepX;
    // float y = lowerY + thisY * stepY;
    int thisX = (blockIdx.x << 6) + (threadIdx.x << 1);
    int thisY = (blockIdx.y << 4) + (threadIdx.y << 1);
    //printf("thisX:%d thisY:%d\n", thisX, thisY);
    for(int i = 0; i < 2; i++)
       for(int j = 0; j < 2; j++){
          float x = lowerX + (thisX + i) * stepX;
          float y = lowerY + (thisY + j) * stepY;
          float z_re = x, z_im = y;
          int ii;
          for (ii = 0; ii < maxIterations; ++ii)
          {
             if (z_re * z_re + z_im * z_im > 4.f)
                break;
             float new_re = z_re * z_re - z_im * z_im;
             float new_im = 2.f * z_re * z_im;
             z_re = x + new_re;
             z_im = y + new_im;
          }
	  int index = (thisY + j) * gridDim.x * blockDim.x * 2 +(thisX + i);
          result[index] = ii;
    }
    
}

// Host front-end function that allocates the memory and launches the GPU kernel
//raccoon:img = output
//raccoon:resX = width
//raccoon:resY = height
void hostFE (float upperX, float upperY, float lowerX, float lowerY, int* img, int resX, int resY, int maxIterations)
{
    float stepX = (upperX - lowerX) / resX;
    float stepY = (upperY - lowerY) / resY;
    /*------------------raccoon------------------------*/
    size_t size = resX * resY * sizeof(int);
    int *temp;
    cudaHostAlloc(&temp, size, cudaHostAllocMapped);
    int *result;
    size_t pitch;
    cudaMallocPitch(&result, &pitch, resX * sizeof(int), resY * sizeof(int));
    cudaMemcpy(result, temp, size, cudaMemcpyHostToDevice);
    dim3 dimBlock(32, 8);
    dim3 dimGrid(resX / dimBlock.x / 2, resY / dimBlock.y / 2);
    //dim3 dimBlock(25,40);
    //dim3 dimGrid(2,2);
    mandelKernel <<<dimGrid, dimBlock>>>(lowerX, lowerY, stepX, stepY, maxIterations, result);
    cudaMemcpy(img, result, size, cudaMemcpyDeviceToHost);
    cudaFree(result);
}
