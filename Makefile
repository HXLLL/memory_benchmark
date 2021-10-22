CC 			:= gcc
CFLAGS		:= -g3
LDFLAGS		:= -lprofiler
LIBS		:= test.c

all: main

main: main.c $(LIBS)
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@ 