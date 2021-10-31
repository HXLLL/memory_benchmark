CC 			:= gcc
CFLAGS		:= -g3
LDFLAGS		:= -lprofiler
LIBS		:= test.c

all: build/main

build/main: main.c $(LIBS)
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@ 

$(shell mkdir -p build)