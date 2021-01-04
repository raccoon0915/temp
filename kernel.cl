__kernel void convolution(float* source_image, float* output_image, float* kernel_filter, int filterWidth, int imageHeight, int imageWidth) 
{
   int halffilterSize = filterWidth / 2;
    float sum;
    int i, j, k, l;

    for (i = 0; i < imageHeight; i++)
    {
        // Iterate over the columns of the source image
        for (j = 0; j < imageWidth; j++)
        {
            sum = 0; // Reset sum for new source pixel
            // Apply the filter to the neighborhood
            for (k = -halffilterSize; k <= halffilterSize; k++)
            {
                for (l = -halffilterSize; l <= halffilterSize; l++)
                {
                    if (i + k >= 0 && i + k < imageHeight &&
                        j + l >= 0 && j + l < imageWidth)
                    {
                        sum += inputImage[(i + k) * imageWidth + j + l] *
                               filter[(k + halffilterSize) * filterWidth +
                                      l + halffilterSize];
                    }
                }
            }
            outputImage[i * imageWidth + j] = sum;
        }
    }
}
