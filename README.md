# GL
OpenGL examples

#### usage

`make`

#### dependencies

* OpenBLAS
* OpenGL
* c++14

#### Visualize points in 3D space

`./test`

<img src="test.gif" />

#### Multiple windows demo + read file + display matrix

`./gltest`

<img src="gltest.png" />

#### CUDA + OpenGL

Create OpenGL texture (256 x 256), then create (256 x 256) grid of CUDA threads and run Conway's Game of Life on GPU

<img src="./CUDAGL/gol.gif"/>
