# $Id: Makefile.am,v 1.5 2003/01/06 17:05:27 christof Exp $

CXXFLAGS=@CXXFLAGS@ @COMMONXX_INCLUDES@
LDFLAGS=@LDFLAGS@  @COMMONXX_LDFLAGS@
LDADD=@COMMONXX_LIBS@

SUBDIRS=graph

#all: $(EXTRA_PROGRAMS)

EXTRA_PROGRAMS=steuerprogramm

BUILT_SOURCES=DynamicEnums

steuerprogramm_SOURCES=steuerprogramm.cc DataBase_init.cc \
      AuftragsVerwaltung.cc Check.cc

CLEANFILES= *~ $(EXTRA_PROGRAMS)

DynamicEnums:
	ln -sf database_tables_init DynamicEnums
