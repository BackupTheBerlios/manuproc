# $Id: Makefile.am,v 1.1 2002/06/20 06:43:31 christof Exp $

CXXFLAGS=@CXXFLAGS@ @COMMONXX_INCLUDES@
LDFLAGS=@LDFLAGS@  @COMMONXX_LDFLAGS@
LDADD=@COMMONXX_LIBS@

all: $(EXTRA_PROGRAMS)

EXTRA_PROGRAMS=steuerprogramm

steuerprogramm_SOURCES=steuerprogramm.cc DataBase_init.cc \
      AuftragsVerwaltung.cc Check.cc

CLEANFILES= *~ $(LINKS) $(EXTRA_PROGRAMS)

$(LINKS):
	ln -sf ../../Basic/examples/$@ .

