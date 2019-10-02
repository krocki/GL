GCC=gcc
TARGETS=test draw readbin cursor
CFLAGS=-g -fPIC
LFLAGS=-framework OpenGL -lpthread -lglfw
OBJECTS:=$(patsubst %.c,%.o,$(wildcard *.c))

all: $(TARGETS)

%.o: %.c
	$(GCC) $(CFLAGS) -c $< -o $@

cursor: cursor.o
	$(GCC) -o $@ $(LFLAGS) $^

readbin: readbin.o
	$(GCC) -o $@ $(LFLAGS) $^

draw: draw.o
	$(GCC) -o $@ $(LFLAGS) $^

test: test.o
	$(GCC) -o $@ $(LFLAGS) $^

clean:
	rm -rf $(TARGETS) $(OBJECTS)
