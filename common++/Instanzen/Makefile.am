# $Id: Makefile.am,v 1.14 2003/05/09 10:19:02 christof Exp $

CXXFLAGS=@ECPG_INCLUDES@ @CXXFLAGS@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_LIBRARIES=libInstanzen.a

includedir=@includedir@/Instanzen

PGCC_Sources=ppsInstanz_sql.cc 

libInstanzen_a_SOURCES= $(PGCC_Sources) Produktionsdauer.cc \
      ppsInstanz.cc 

libInstanzen.a: $(libInstanzen_a_OBJECTS) $(libInstanzen_a_DEPENDENCIES)
	(for i in $(libInstanzen_a_OBJECTS) ; do echo Instanzen/$$i ; done ) >$@

#LINKS= 

#$(LINKS):
#	ln -sf ../Basic/src/$@ .

BUILT_SOURCES= $(PGCC_Sources) 

include_HEADERS=ppsInstanz.h 

CLEANFILES= *~ $(BUILT_SOURCES)

