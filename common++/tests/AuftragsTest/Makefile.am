# $Id: Makefile.am,v 1.7 2003/07/08 07:14:05 christof Exp $

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

DynamicEnums:
#	ln -sf database_tables_init DynamicEnums # petig
	ln -sf ../../../Anleitung/datenbank DynamicEnums # anleitung
