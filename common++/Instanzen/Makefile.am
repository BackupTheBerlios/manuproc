# $Id: Makefile.am,v 1.10 2002/11/07 07:50:18 christof Exp $

CXXFLAGS=@ECPG_INCLUDES@ @CXXFLAGS@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_LIBRARIES=libInstanzen.a

includedir=@includedir@/Instanzen

PGCC_Sources=ppsInstanz_sql.cc 

libInstanzen_a_SOURCES= $(PGCC_Sources) Produktionsdauer.cc \
      ppsInstanzProduziert.cc ppsInstanz.cc

libInstanzen.a: $(libInstanzen_a_OBJECTS) $(libInstanzen_a_DEPENDENCIES)
	touch $@

#LINKS= 

#$(LINKS):
#	ln -sf ../Basic/src/$@ .

BUILT_SOURCES= $(PGCC_Sources) 

include_HEADERS=ppsInstanz.h ppsInstanzProduziert.h

CLEANFILES= *~ $(BUILT_SOURCES)

