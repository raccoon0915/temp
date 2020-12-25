 __global__ void my2DConvKernel(float *d_Result, float *d_Data, int dataW, int dataH)
 {
   // original image based coordinate
   int y = blockIdx.y * blockDim.y   threadIdx.y;
  int x = blockIdx.x * blockDim.x   threadIdx.x;
 
   int  BiasY = y - KERNEL_RADIUS;
   int  BiasX = x - KERNEL_RADIUS;
 
   float sum = 0;
   for(int j = 0; j &lt; KERNEL_LENGTH;   j)
   {
     //out of image range
     if (BiasY   j &lt; 0 || BiasY   j &gt;= dataH) 
       continue;
 
     for(int i = 0; i &lt; KERNEL_LENGTH;   i)
     {
       //out of image range
       if (BiasX   i &lt; 0 || BiasX   i &gt;= dataW) 
         continue;
 
       sum  = d_Data[(BiasY   j) * dataW   BiasX   i] * 
              c_Kernel[KERNEL_LENGTH * j   i];
     }
   }
 
   d_Result[y * dataW   x] = sum;  
 }
