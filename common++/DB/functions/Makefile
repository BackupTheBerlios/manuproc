CFLAGS=-Wall -fPIC  \
	-I$(HOME)/build/pgsql/src/include \
	-I$(HOME)/cvs/BarcoNG/include \
	-I$(HOME)/pgsql-cvs/pgsql-stable/src/include \
	-I/usr/include/postgresql/server \
	-I/usr/include/postgresql \
	-I/usr/local/pgsql/include \
	-I/usr/local/postgresql/include 

ALL=nextval_tnf.so auftrag_status.so artikelbezeichnung.so kw2date.so \
	atoi.so artikelid.so

all: $(ALL)

hex.so: hex.o msw.o
	ld -shared -o hex.so $^

%.so: %.o
	ld -shared -o $*.so $^

clean:
	rm -f *~ *.o $(ALL)

install:
	cp auftrag_status.h /home/share/include
	cp $(ALL) /usr/local/pgsql/lib

