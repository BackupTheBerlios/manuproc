# $Id: Makefile.am,v 1.2 2002/09/26 14:55:53 thoma Exp $

CXXFLAGS=@ECPG_INCLUDES@ @CXXFLAGS@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_LIBRARIES=libInstanzen.a

includedir=@includedir@/Instanzen

PGCC_Sources=ppsInstanz_sql.cc 

libInstanzen_a_SOURCES= $(PGCC_Sources) Produziert.cc Produktionsdauer.cc

libInstanzen.a: $(libInstanzen_a_OBJECTS) $(libInstanzen_a_DEPENDENCIES)
	touch $@

#LINKS= 

#$(LINKS):
#	ln -sf ../Basic/src/$@ .

BUILT_SOURCES= $(PGCC_Sources) 

include_HEADERS=ppsInstanz.h Produziert.h

CLEANFILES= *~ $(BUILT_SOURCES)

