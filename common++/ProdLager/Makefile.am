# $Id: Makefile.am,v 1.3 2002/01/17 08:25:53 cvs_malte Exp $

ENDUSER=-DPETIG

CXXFLAGS=@ECPG_INCLUDES@ @CXXFLAGS@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_PROGRAMS=libProdLager.o

includedir=@includedir@/ProdLager

libProdLager_o_SOURCES= $(PGCC_SOURCES) ProdLager.cc Lager.cc LagerPlatz.cc JumboRolle.cc

libProdLager.o: $(libProdLager_o_OBJECTS) $(libProdLager_o_DEPENDENCIES)
	touch $@

PGCC_SOURCES= ProdLager_sql.cc Lager_sql.cc maschinen_geschwindigkeit_sql.cc JumboRolle_sql.cc

BUILT_SOURCES= $(PGCC_SOURCES)

include_HEADERS = ProdLager.hh LagerPlatz.hh \
   maschinen_geschwindigkeit.hh Lager.hh JumboRolle.h CodeError.h

CLEANFILES= *~ $(BUILT_SOURCES) 

