# $Id: Makefile.am,v 1.3 2002/06/20 06:29:53 christof Exp $

CXXFLAGS=@CXXFLAGS@ @ECPG_INCLUDES@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_LIBRARIES=libBank.a

includedir=@includedir@/Bank

libBank_a_SOURCES=Bankauftrag.cc Datenfehler.cc \
	Zahlvorgang.cc $(BUILT_SOURCES)

libBank.a: $(libBank_a_OBJECTS) $(libBank_a_DEPENDENCIES)
	touch $@

BUILT_SOURCES=Bankauftrag2.cc

include_HEADERS=Bankauftrag.h Datenfehler.h Zahlvorgang.h

CLEANFILES= *~ *.pct *.prot $(BUILT_SOURCES)

