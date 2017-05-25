# Makefile

# vars
PROGRAM = sample
OBJS    = sample.o path_normalize.o
CC      = gcc
CFLAGS  = -Wall -Os -march=native

VERSION  = v1.00

# rules

.SUFFIXES: .c .o

.PHONY: all
all:$(PROGRAM)

sample: $(OBJS)
	$(CC) -o $@ $(OBJS) -Wall


sample.o:         sample.c path_normalize.h
path_normalize.o: path_normalize.c path_normalize.h

.c .o:
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	$(RM) $(PROGRAM) $(OBJS)
