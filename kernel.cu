#include <cuda.h>
#include <stdio.h>
#include <stdlib.h>

__global__ void convolution(float* inputImage , float* outputImage, float* filter, int imageWidth, int imageHeight, int filterWidth) {
    // To avoid error caused by the floating number, use the following pseudo code
    //
    // float x = lowerX + thisX * stepX;
    // float y = lowerY + thisY * stepY;
    int i = blockIdx.y * blockDim.y + threadIdx.y; /*raccoon: y = row*/	
    int j = blockIdx.x * blockDim.x + threadIdx.x; /*raccoon: x = column*/
    float sum = 0;
    int halffilterSize = filterWidth / 2;
    int k, l;
    for(k = -halffilterSize; k <= haltfilterSize; k++){
	    for(l = halffilterSize; l <=halffilterSize; l++){
		    if(i + k >= 0 && i + k < imageHeight && j + l >= 0 && j + l < imageWidth){
			    sum += inputImage[(i + k) * imageWidth + j + l] * 
				    filter[(k + halffilterSize) * filterWidth + l + halffilterSize];
		    }
	    }
    }
    outputImage[i * imageWidth + j] = sum;    
}
// Host front-end function that allocates the memory and launches the GPU kernel
//raccoon:img = output
//raccoon:resX = width
//raccoon:resY = height
void hostFE (float* inputImage, float* outputImage, float* filter, int imageWidth, int imageHeight, int filterWidth)
{
    /*------------------raccoon------------------------*/
    size_t imageSize = imageHeight * imageWidth * sizeof(float);
    size_t filterSize = filterWidth * filterWidth * sizeof(float);
    float* source_image;
    float* output_image;
    float* kernel_filter;
    cudaMalloc(&source_image, imageSize);
    cudaMalloc(&output_image, imageSize);
    cudaMalloc(&kernel_filter, filterSize);
    cudaMemcpy(source_image, inputImage, imageSize, cudaMemcpyHostToDevice);
    cudaMemcpy(output_image, outputImage, imageSize, cudaMemcpyHostToDevice);
    cudaMemcpy(kernel_filter, filter, filterSize, cudaMemcpyHostToDevice);
    dim3 dimBlock(8, 8);
    dim3 dimGrid(imageWidth / dimBlock.x, imageHeight / dimBlock.y);
    convolution <<<dimGrid, dimBlock>>>(, lowerY, stepX, stepY, maxIterations, result);
    cudaMemcpy(img, result, size, cudaMemcpyDeviceToHost);
    cudaFree(result);
}
