CXX=g++

# OS Detection
OS := $(shell uname)
ARCH := $(shell uname -m)

# OS X
ifeq ($(OS), Darwin)
	INCLUDES=-I./src -I/usr/local/opt/openblas/include
	LFLAGS = -lopenblas -L/usr/local/opt/openblas/lib
	LFLAGS := $(LFLAGS) -framework GLUT -framework OpenGL -framework Cocoa
endif

# Linux
ifeq ($(OS), Linux)
	INCLUDES=-I./src -I/opt/OpenBLAS/include
	LFLAGS=-lopenblas -L/opt/OpenBLAS/lib -lpthread
	LFLAGS := $(LFLAGS) -lGL -lGLU -lglut
endif

CFLAGS=-std=c++14 -O3 -DUSE_OPENGL -w

all:
	$(CXX) ./src/main.cc $(INCLUDES) $(CFLAGS) $(LFLAGS) -o test
	$(CXX) ./src/matrix.cc $(INCLUDES) $(CFLAGS) $(LFLAGS) -o matrix
	$(CXX) ./src/gltest.cc $(INCLUDES) $(CFLAGS) $(LFLAGS) -o gltest
