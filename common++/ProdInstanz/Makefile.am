# $Id: Makefile.am,v 1.2 2001/09/06 08:16:23 cvs_malte Exp $

CXXFLAGS=@ECPG_INCLUDES@ @CXXFLAGS@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_PROGRAMS=libProdInstanz.o

includedir=@includedir@/ProdInstanz

libProdInstanz_o_SOURCES= $(PGCC_SOURCES) $(BUILT_SOURCES)

libProdInstanz.o: $(libProdInstanz_o_OBJECTS) $(libProdInstanz_o_DEPENDENCIES)
	touch $@

PGCC_SOURCES= ProdInstanz_sql.cc maschinen_geschwindigkeit_sql.cc

BUILT_SOURCES= $(PGCC_SOURCES) ProdInstanz.cc

include_HEADERS = ProdInstanz.hh maschinen_geschwindigkeit.hh

CLEANFILES= *~ $(BUILT_SOURCES)

