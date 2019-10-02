GCC=gcc
TARGETS=test draw
CFLAGS=-g -fPIC
LFLAGS=-framework OpenGL -lpthread -lglfw
OBJECTS:=$(patsubst %.c,%.o,$(wildcard *.c))

all: $(TARGETS)

%.o: %.c
	$(GCC) $(CFLAGS) -c $< -o $@

draw: draw.o
	$(GCC) -o $@ $(LFLAGS) $^

test: test.o
	$(GCC) -o $@ $(LFLAGS) $^

clean:
	rm -rf $(TARGETS) $(OBJECTS)
