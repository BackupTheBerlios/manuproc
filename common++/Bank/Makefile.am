# $Id: Makefile.am,v 1.5 2003/05/09 10:19:02 christof Exp $

CXXFLAGS=@CXXFLAGS@ @ECPG_INCLUDES@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_LIBRARIES=libBank.a

includedir=@includedir@/Bank

libBank_a_SOURCES=Bankauftrag.cc Datenfehler.cc \
	Zahlvorgang.cc Ueberweisung.cc $(BUILT_SOURCES)

libBank.a: $(libBank_a_OBJECTS) $(libBank_a_DEPENDENCIES)
	(for i in $(libBank_a_OBJECTS) ; do echo Bank/$$i ; done ) >$@

BUILT_SOURCES=Bankauftrag2.cc

include_HEADERS=Bankauftrag.h Datenfehler.h Zahlvorgang.h Ueberweisung.h

CLEANFILES= *~ *.pct *.prot $(BUILT_SOURCES)

