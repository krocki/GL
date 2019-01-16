all: gameoflife

main.o: main.cpp
	nvcc -ccbin g++ -m64 -o main.o -c main.cpp
gameoflife.o: gameoflife.cu
	nvcc -ccbin g++ -m64 -o gameoflife.o -c gameoflife.cu
gameoflife: main.o gameoflife.o
	nvcc -ccbin g++ -m64 -o gameoflife main.o gameoflife.o -lGL -lGLU -lglut
clean:
	rm *.o gameoflife
