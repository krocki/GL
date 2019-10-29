GCC=gcc
TARGETS=test draw readbin cursor points nearest util button graph graph_util split quad
CFLAGS=-g -fPIC --std=c11
LFLAGS=-framework OpenGL -lpthread -lglfw
OBJECTS:=$(patsubst %.c,%.o,$(wildcard *.c))

.SUFFIXES:

all: $(TARGETS)

%.o: %.c
	$(GCC) $(CFLAGS) -c $< -o $@

%: %.o
	$(GCC) -o $@ $(LFLAGS) $^

clean:
	rm -rf $(TARGETS) $(OBJECTS)
