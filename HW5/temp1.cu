#include <cuda.h>
#include <stdio.h>
#include <stdlib.h>

__global__ void mandelKernel(float lowerX, float lowerY, float stepX, float stepY, int maxIterations, int* result) {
    // To avoid error caused by the floating number, use the following pseudo code
    //
    // float x = lowerX + thisX * stepX;
    // float y = lowerY + thisY * stepY;
    int thisX = blockIdx.x * blockDim.x + threadIdx.x;
    int thisY = blockIdx.y * blockDim.y + threadIdx.y;
    float x = lowerX + thisX * stepX;
    float y = lowerY + thisY * stepY;
    float z_re = x, z_im = y;
    int i;
    for (i = 0; i < maxIterations; ++i)
    {

      if (z_re * z_re + z_im * z_im > 4.f)
        break;

      float new_re = z_re * z_re - z_im * z_im;
      float new_im = 2.f * z_re * z_im;
      z_re = x + new_re;
      z_im = y + new_im;
    }
    result[thisY * gridDim.x * blockDim.x + thisX] = i;
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
    int *temp = (int*)malloc(size);
    int* result;
    cudaMalloc(&result, size);
    cudaMemcpy(result, temp, size, cudaMemcpyHostToDevice);
    dim3 dimBlock(40, 25);
    dim3 dimGrid(resX / dimBlock.x, resY / dimBlock.y);
    mandelKernel <<<dimGrid, dimBlock>>>(lowerX, lowerY, stepX, stepY, maxIterations, result);
    cudaMemcpy(img, result, size, cudaMemcpyDeviceToHost);
    cudaFree(result);
}
