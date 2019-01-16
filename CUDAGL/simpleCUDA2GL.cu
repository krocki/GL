/*
 * Copyright 1993-2015 NVIDIA Corporation.  All rights reserved.
 *
 * Please refer to the NVIDIA end user license agreement (EULA) associated
 * with this source code for terms and conditions that govern your use of
 * this software. Any use, reproduction, disclosure, or distribution of
 * this software and related documentation outside the terms of the EULA
 * is strictly prohibited.
 *
 */

// Utilities and system includes

// clamp x to range [a, b]
__device__ float clamp(float x, float a, float b)
{
    return max(a, min(b, x));
}

__device__ int clamp(int x, int a, int b)
{
    return max(a, min(b, x));
}

// convert floating point rgb color to 8-bit integer
__device__ int rgbToInt(float r, float g, float b)
{
    r = clamp(r, 0.0f, 255.0f);
    g = clamp(g, 0.0f, 255.0f);
    b = clamp(b, 0.0f, 255.0f);
    return (int(b)<<16) | (int(g)<<8) | int(r);
}

__global__ void cudainit(unsigned int *g_odata, int imgw) {

    int tx = threadIdx.x;
    int ty = threadIdx.y;
    int bw = blockDim.x;
    int bh = blockDim.y;
    int x = blockIdx.x*bw + tx;
    int y = blockIdx.y*bh + ty;

    g_odata[y*imgw+x] = (tx + ty + bw + bh ) % 7 == 0 ? 0xffffffff : 0x00000000;
}

__global__ void cudaProcess(unsigned int *g_odata, int imgw)
{
    extern __shared__ uchar4 sdata[];

    int tx = threadIdx.x;
    int ty = threadIdx.y;
    int bw = blockDim.x;
    int bh = blockDim.y;
    int x = blockIdx.x*bw + tx;
    int y = blockIdx.y*bh + ty;

    unsigned int in = g_odata[y*imgw+x];
    unsigned int out;
    unsigned int n=0;

    // counting neighbors
    if (y > 0        && x > 0        ) n += (g_odata[(y-1)*imgw+(x-1)] > 0) ? 1 : 0;
    if (y > 0                        ) n += (g_odata[(y-1)*imgw+(x  )] > 0) ? 1 : 0;
    if (y > 0        && x < (imgw-1) ) n += (g_odata[(y-1)*imgw+(x+1)] > 0) ? 1 : 0;
    if (                x > 0        ) n += (g_odata[(y  )*imgw+(x-1)] > 0) ? 1 : 0;
    if (                x < (imgw-1) ) n += (g_odata[(y  )*imgw+(x+1)] > 0) ? 1 : 0;
    if (y < (imgw-1) && x > 0        ) n += (g_odata[(y+1)*imgw+(x-1)] > 0) ? 1 : 0;
    if (y < (imgw-1)                 ) n += (g_odata[(y+1)*imgw+(x  )] > 0) ? 1 : 0;
    if (y < (imgw-1) && x < (imgw-1) ) n += (g_odata[(y+1)*imgw+(x+1)] > 0) ? 1 : 0;

    // rules of game of life
    if (n < 2 || n > 3) out = 0;
    else if (n == 3 && g_odata[(y)*imgw+(x)] == 0) out = 0xffffffff;
    else out = in;

    g_odata[y*imgw+x] = out;//rgbToInt(c4.z, c4.y, c4.x);
}

extern "C" void launch_cudainit(dim3 grid, dim3 block, int sbytes, unsigned int *g_odata, int imgw)
{
    cudainit<<< grid, block, sbytes >>>(g_odata, imgw);

}

extern "C" void launch_cudaProcess(dim3 grid, dim3 block, int sbytes, unsigned int *g_odata, int imgw)
{
    cudaProcess<<< grid, block, sbytes >>>(g_odata, imgw);

}
