# $Id: Makefile.am,v 1.2 2002/01/10 08:00:03 cvs_malte Exp $

ENDUSER=-DPETIG

CXXFLAGS=@ECPG_INCLUDES@ @CXXFLAGS@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_PROGRAMS=libProdLager.o

includedir=@includedir@/ProdLager

libProdLager_o_SOURCES= $(PGCC_SOURCES) ProdLager.cc Lager.cc LagerPlatz.cc

libProdLager.o: $(libProdLager_o_OBJECTS) $(libProdLager_o_DEPENDENCIES)
	touch $@

PGCC_SOURCES= ProdLager_sql.cc Lager_sql.cc maschinen_geschwindigkeit_sql.cc

BUILT_SOURCES= $(PGCC_SOURCES)

include_HEADERS = ProdLager.hh LagerPlatz.hh \
   maschinen_geschwindigkeit.hh Lager.hh

CLEANFILES= *~ $(BUILT_SOURCES) 

