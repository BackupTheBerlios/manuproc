# $Id: Makefile.am,v 1.16 2003/07/30 09:24:54 christof Exp $

AM_CXXFLAGS=@ECPG_INCLUDES@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_LIBRARIES=libInstanzen.a

includedir=@includedir@/Instanzen

PGCC_Sources=ppsInstanz_sql.cc 

libInstanzen_a_SOURCES= $(PGCC_Sources) Produktionsdauer.cc \
      ppsInstanz.cc 

libInstanzen.a: $(libInstanzen_a_OBJECTS) $(libInstanzen_a_DEPENDENCIES) Makefile
	(for i in $(libInstanzen_a_OBJECTS) ; do echo Instanzen/$$i ; done ) >$@

#LINKS= 

#$(LINKS):
#	ln -sf ../Basic/src/$@ .

BUILT_SOURCES= $(PGCC_Sources) 

include_HEADERS=ppsInstanz.h 

CLEANFILES= *~ $(BUILT_SOURCES)

