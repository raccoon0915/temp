#include <stdio.h>
#include <stdlib.h>
#include "hostFE.h"
#include "helper.h"

void hostFE(int filterWidth, float *filter, int imageHeight, int imageWidth,
            float *inputImage, float *outputImage, cl_device_id *device,
            cl_context *context, cl_program *program)
{
    cl_int status;
    int filterSize = filterWidth * filterWidth;
    //raccoon:Load the kernel code into the array source_str
    FILE *fp;
    char *source_str;
    size_t source_size;
 
    fp = fopen("kernel.cl", "r");
    if (!fp) {
        fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }
    source_str = (char*)malloc(MAX_SOURCE_SIZE);
    source_size = fread( source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose( fp );
 
    // Get platform and device information
    cl_platform_id platform_id = NULL;   
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs( platform_id, CL_DEVICE_TYPE_DEFAULT, 1, 
            device, &ret_num_devices);/*raccoon:change device_id*/
 
    // Create an OpenCL context raccoon:TA has provided
    *context = clCreateContext( NULL, 1, device, NULL, NULL, &ret);
 
    // Create a command queue
    cl_command_queue command_queue = clCreateCommandQueue(*context, *device, 0, &ret);
 
    // Create memory buffers on the device for each vector 
    cl_mem source_image = clCreateBuffer(*context, CL_MEM_READ_ONLY, 
            imageWidth * imageHeight *sizeof(float), NULL, &ret);
    cl_mem output_image = clCreateBuffer(*context, CL_MEM_READ_ONLY,
            imageWidth * imageHeight *sizeof(float), NULL, &ret);
    cl_mem kernel_filter = clCreateBuffer(*context, CL_MEM_WRITE_ONLY, 
            filterWidth * filterWidth * sizeof(float), NULL, &ret);
    
    // Copy the inputImage and filter to their respective memory buffers
    ret = clEnqueueWriteBuffer(command_queue, source_image, CL_TRUE, 0,
            imageWidth * imageHeight * sizeof(float), inputImage, 0, NULL, NULL);
    ret = clEnqueueWriteBuffer(command_queue, kernel_filter, CL_TRUE, 0, 
            filterWidth * filterWidth * sizeof(float), filter, 0, NULL, NULL);
 
    // Create a program from the kernel source
    program = clCreateProgramWithSource(*context, 1, 
            (const char **)&source_str, (const size_t *)&source_size, &ret);
 
    // Build the program
    ret = clBuildProgram(program, 1, device, NULL, NULL, NULL);
 
    // Create the OpenCL kernel
    cl_kernel kernel = clCreateKernel(program, "convolution", &ret);
 
    // Set the arguments of the kernel
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&source_image);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&output_image);
    ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&kernel_filter);
    ret = clSetKernelArg(kernel, 3, sizeof(int), (void*)&filterWidth);
    ret = clSetKernelArg(kernel, 4, sizeof(int), (void*)&imageHeight);
    ret = clSetKernelArg(kernel, 5, sizeof(int), (void*)&imageWidth);
 
    // Execute the OpenCL kernel on the list
    size_t global_item_size = imageHeight * imageWidth; // Process the entire matrix
    size_t local_item_size = 64; // Divide work items into groups of 64
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, 
            &global_item_size, &local_item_size, 0, NULL, NULL);
 
    // Read the memory buffer C on the device to the local variable C
    //float *result = (float*)malloc(sizeof(float) * imageHeiht * imageWidth);
    ret = clEnqueueReadBuffer(command_queue, output_image, CL_TRUE, 0, 
            imageHeight * imageWidth * sizeof(float), outputImage, 0, NULL, NULL);
 
    // Display the result to the screen
    //for(i = 0; i < LIST_SIZE; i++)
        printf("%d + %d = %d\n", A[i], B[i], C[i]);
 
    // Clean up
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(source_image);
    ret = clReleaseMemObject(output_image);
    ret = clReleaseMemObject(kernel_filter);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);
    //free(source_image);
    //free(output_image);
    //free(kernel_filter);
    return 0;
    
    
    
    
    
    
    
}
