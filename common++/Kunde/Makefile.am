CXXFLAGS=@CXXFLAGS@ @ECPG_INCLUDES@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_PROGRAMS=libKunde.o

includedir=@includedir@/Kunde

libKunde_o_SOURCES=Kunde.cc Person.cc $(BUILT_SOURCES)

BUILT_SOURCES=Kunde_sql.cc Person_sql.cc Telefon.cc

include_HEADERS=Kunde.h Telefon.h Person.h

CLEANFILES= *~ *.pct *.prot $(BUILT_SOURCES)

