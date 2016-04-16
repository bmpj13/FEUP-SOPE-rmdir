CC = gcc
CFLAGS  = -Wall

OBJECTS = rmdup.o lssort.o lsdir.o 


all: rmdup lssort lsdir

rmdup:    rmdup.o
	  cc bin/rmdup.o -o bin/rmdup $(CFLAGS)
	  
	  
lssort:   lssort.o
	  cc bin/lssort.o -o bin/lssort $(CFLAGS)
	  
	  
lsdir:    lsdir.o
	  cc bin/lsdir.o -o bin/lsdir $(CFLAGS)


	  
lsdir.o:  lsdir.c
	  cc -c lsdir.c -o bin/lsdir.o $(CFLAGS)


lssort.o: lssort.c
	  cc -c lssort.c -o bin/lssort.o $(CFLAGS)


rmdup.o:  rmdup.c rmdup.h
	  cc -c rmdup.c -o bin/rmdup.o $(CFLAGS)
