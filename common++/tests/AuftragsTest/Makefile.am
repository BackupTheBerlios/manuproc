# $Id: Makefile.am,v 1.3 2002/07/15 15:37:53 christof Exp $

CXXFLAGS=@CXXFLAGS@ @COMMONXX_INCLUDES@
LDFLAGS=@LDFLAGS@  @COMMONXX_LDFLAGS@
LDADD=@COMMONXX_LIBS@

SUBDIRS=graph

all: $(EXTRA_PROGRAMS)

EXTRA_PROGRAMS=steuerprogramm

BUILT_SOURCES=DynamicEnums

steuerprogramm_SOURCES=steuerprogramm.cc DataBase_init.cc \
      AuftragsVerwaltung.cc Check.cc

CLEANFILES= *~ $(EXTRA_PROGRAMS)

DynamicEnums:
	ln -sf . DynamicEnums
