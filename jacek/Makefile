# $Id: 

CC=g++
CFLAGS=-Wall -g -O0 -I$(HOME)/cvs/c++ -I/usr/include/postgresql
LFLAGS=-L$(HOME)/cvs/c++
LIBS=libcommon++.a

ean_set: ean_set.cc
	$(CC) $(CFLAGS) -o ean_set ean_set.cc $(LFLAGS)\
	-lecpg  -lcommon++ 

clean:
	-rm -f *~ core ean_set *.o


