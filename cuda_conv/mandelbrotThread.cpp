#include "kernel.h"

//
// MandelbrotThread --
//
// Multi-threaded implementation of mandelbrot set image generation.
// Threads of execution are created by using CUDA
void mandelbrotThread(float* inputImage, float* outputImage, float* filter, int imageWidth, int imageHeight, int filterWidth)
{
    hostFE(inputImage, outputImage, filter, imageWidth, imageHeight, filterWidth);
}
