CXXFLAGS=@CXXFLAGS@ @ECPG_INCLUDES@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_PROGRAMS=libBank.o

includedir=@includedir@/Bank

libBank_o_SOURCES=Bankauftrag.cc Datenfehler.cc \
	Zahlvorgang.cc $(BUILT_SOURCES)

libBank.o: $(libBank_o_OBJECTS) $(libBank_o_DEPENDENCIES)
	touch $@

BUILT_SOURCES=Bankauftrag2.cc

include_HEADERS=Bankauftrag.h Datenfehler.h Zahlvorgang.h

CLEANFILES= *~ *.pct *.prot $(BUILT_SOURCES)

