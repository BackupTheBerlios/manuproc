# $Id: 

CC=g++
CFLAGS=-Wall -g -O0 -I$(HOME)/cvs/c++ -I/usr/include/postgresql
LFLAGS=-L$(HOME)/cvs/c++
LIBS=libcommon++.a

scan_list: scan_list.cc
	$(CC) $(CFLAGS) -o scan_list scan_list.cc $(LFLAGS)\
	-lecpg  -lcommon++ 

clean:
	-rm -f *~ core scan_list *.o


