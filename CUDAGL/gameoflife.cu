// kmrocki 1/15/19

__global__ void cudainit(unsigned int *canvas, int imgw) {

    unsigned int x = blockIdx.x*blockDim.x + threadIdx.x;
    unsigned int y = blockIdx.y*blockDim.y + threadIdx.y;

    canvas[y*imgw+x] = (threadIdx.x + threadIdx.y + blockDim.x + blockDim.y ) % 7 == 0 ? 0xffffffff : 0x00000000;
}

__global__ void gameoflife(unsigned int *canvas, int imgw)
{
    // thread indices
    unsigned int x = blockIdx.x*blockDim.x + threadIdx.x;
    unsigned int y = blockIdx.y*blockDim.y + threadIdx.y;

    // original value at location y,x
    unsigned int in = canvas[y*imgw+x];
    unsigned int n=0; // number of neighbors

    // counting neighbors
    if (y > 0        && x > 0        ) n += (canvas[(y-1)*imgw+(x-1)] > 0) ? 1 : 0;
    if (y > 0                        ) n += (canvas[(y-1)*imgw+(x  )] > 0) ? 1 : 0;
    if (y > 0        && x < (imgw-1) ) n += (canvas[(y-1)*imgw+(x+1)] > 0) ? 1 : 0;
    if (                x > 0        ) n += (canvas[(y  )*imgw+(x-1)] > 0) ? 1 : 0;
    if (                x < (imgw-1) ) n += (canvas[(y  )*imgw+(x+1)] > 0) ? 1 : 0;
    if (y < (imgw-1) && x > 0        ) n += (canvas[(y+1)*imgw+(x-1)] > 0) ? 1 : 0;
    if (y < (imgw-1)                 ) n += (canvas[(y+1)*imgw+(x  )] > 0) ? 1 : 0;
    if (y < (imgw-1) && x < (imgw-1) ) n += (canvas[(y+1)*imgw+(x+1)] > 0) ? 1 : 0;

    // rules of game of life
    unsigned int out;
    if (n < 2 || n > 3) out = 0;
    else if (n == 3 && canvas[(y)*imgw+(x)] == 0) out = 0xffffffff;
    else out = in;

    canvas[y*imgw+x] = out;
}

extern "C" void launch_cudainit(dim3 grid, dim3 block, int sbytes, unsigned int *canvas, int imgw)
{
    cudainit<<< grid, block, sbytes >>>(canvas, imgw);

}

extern "C" void launch_process(dim3 grid, dim3 block, int sbytes, unsigned int *canvas, int imgw)
{
    gameoflife<<< grid, block, sbytes >>>(canvas, imgw);

}
