# $Id: Makefile.am,v 1.4 2003/04/24 10:21:51 jacek Exp $

CXXFLAGS=@CXXFLAGS@ @ECPG_INCLUDES@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_LIBRARIES=libBank.a

includedir=@includedir@/Bank

libBank_a_SOURCES=Bankauftrag.cc Datenfehler.cc \
	Zahlvorgang.cc Ueberweisung.cc $(BUILT_SOURCES)

libBank.a: $(libBank_a_OBJECTS) $(libBank_a_DEPENDENCIES)
	touch $@

BUILT_SOURCES=Bankauftrag2.cc

include_HEADERS=Bankauftrag.h Datenfehler.h Zahlvorgang.h Ueberweisung.h

CLEANFILES= *~ *.pct *.prot $(BUILT_SOURCES)

