# $Id: Makefile.am,v 1.12 2004/02/27 10:34:15 christof Exp $

AM_CXXFLAGS=@COMMONXX_INCLUDES@
AM_LDFLAGS=@COMMONXX_LDFLAGS@
LDADD=@COMMONXX_LIBS@

SUBDIRS=graph

#all: $(EXTRA_PROGRAMS)

EXTRA_PROGRAMS=steuerprogramm

BUILT_SOURCES=DynamicEnums

steuerprogramm_SOURCES=steuerprogramm.cc DataBase_init.cc \
	AuftragsVerwaltung.cc Check.cc petig_tests.cc common_tests.cc \
	mabella_tests.cc manuproc_tests.cc petig_tests2.cc

CLEANFILES= *~ $(EXTRA_PROGRAMS)

DynamicEnums: ../../ManuProCConfig.h
	rm -f DynamicEnums && ln -s `./where_are_the_tables` DynamicEnums
