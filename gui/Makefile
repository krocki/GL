GCC=gcc
TARGETS=logread test draw readbin cursor points nearest util button graph graph_util split quad quad_orig
CFLAGS=-g3 -fPIC --std=c11
INCLUDES=../../clib
LFLAGS=-framework OpenGL -lpthread -lglfw
OBJECTS:=$(patsubst %.c,%.o,$(wildcard *.c))

.SUFFIXES:

all: $(TARGETS)

%.o: %.c
	$(GCC) -I$(INCLUDES) $(CFLAGS) -c $< -o $@

%: %.o
	$(GCC) -o $@ $(LFLAGS) $^

clean:
	rm -rf $(TARGETS) $(OBJECTS)
