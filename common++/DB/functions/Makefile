CFLAGS=-Wall -fPIC  \
	-I/usr/include/postgresql/server \
	-I/usr/include/postgresql \
	-I/usr/lib/postgresql/include 

ALL=auftrag_status.so artikelbezeichnung.so kw2date.so \
	atoi.so artikelid.so

all: $(ALL)

links:
	ln -s ../../Auftrag/auftrag_status.h

%.so: %.o
	ld -shared -o $*.so $^

clean:
	rm -f *~ *.o $(ALL)

install:
	cp $(ALL) /usr/lib/postgresql/lib

