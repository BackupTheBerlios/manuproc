CFLAGS=-Wall -fPIC  \
	-I/usr/include/postgresql/server \
	-I/usr/include/postgresql/8.0/server \
	-I/usr/include/postgresql \
	-I/usr/lib/postgresql/include

ALL=auftrag_status.so artikelbezeichnung.so kw2date.so \
	atoi.so artikelid.so artikelkomponente.so ean_check.so

all: auftrag_status.h $(ALL)

links: auftrag_status.h

auftrag_status.h:
	ln -s ../../Auftrag/auftrag_status.h

%.so: %.o
	ld -shared -o $*.so $^

clean:
	rm -f *~ *.o $(ALL)

install:
	mkdir -p /usr/lib/postgresql/lib
	cp $(ALL) /usr/lib/postgresql/lib

