# $Id: Makefile.am,v 1.3 2001/10/04 09:06:41 cvs_malte Exp $

CXXFLAGS=@ECPG_INCLUDES@ @CXXFLAGS@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_PROGRAMS=libProdInstanz.o

includedir=@includedir@/ProdInstanz

libProdInstanz_o_SOURCES= $(PGCC_SOURCES) $(BUILT_SOURCES)

libProdInstanz.o: $(libProdInstanz_o_OBJECTS) $(libProdInstanz_o_DEPENDENCIES)
	touch $@

PGCC_SOURCES= ProdInstanz_sql.cc maschinen_geschwindigkeit_sql.cc

BUILT_SOURCES= $(PGCC_SOURCES)

include_HEADERS = ProdInstanz.hh maschinen_geschwindigkeit.hh

CLEANFILES= *~ $(BUILT_SOURCES)

