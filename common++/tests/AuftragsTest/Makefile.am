# $Id: Makefile.am,v 1.9 2003/07/16 12:02:23 christof Exp $

AM_CXXFLAGS=@COMMONXX_INCLUDES@
AM_LDFLAGS=@COMMONXX_LDFLAGS@
LDADD=@COMMONXX_LIBS@

SUBDIRS=graph

#all: $(EXTRA_PROGRAMS)

EXTRA_PROGRAMS=steuerprogramm

BUILT_SOURCES=DynamicEnums

steuerprogramm_SOURCES=steuerprogramm.cc DataBase_init.cc \
	AuftragsVerwaltung.cc Check.cc petig_tests.cc common_tests.cc \
	mabella_tests.cc manuproc_tests.cc

CLEANFILES= *~ $(EXTRA_PROGRAMS)

DynamicEnums: ../../ManuProCConfig.h
	ln -sf `./where_are_the_tables` DynamicEnums
