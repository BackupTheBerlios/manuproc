# $Id: Makefile.am,v 1.8 2003/07/14 07:16:10 christof Exp $

AM_CXXFLAGS=@COMMONXX_INCLUDES@
AM_LDFLAGS=@COMMONXX_LDFLAGS@
LDADD=@COMMONXX_LIBS@

SUBDIRS=graph

#all: $(EXTRA_PROGRAMS)

EXTRA_PROGRAMS=steuerprogramm

BUILT_SOURCES=DynamicEnums

steuerprogramm_SOURCES=steuerprogramm.cc DataBase_init.cc \
      AuftragsVerwaltung.cc Check.cc

CLEANFILES= *~ $(EXTRA_PROGRAMS)

DynamicEnums: ../../ManuProCConfig.h
	ln -sf `./where_are_the_tables` DynamicEnums
