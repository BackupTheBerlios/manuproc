# $Id: Makefile.am,v 1.1 2002/02/08 21:51:26 christof Exp $

ENDUSER=-DPETIG

CXXFLAGS=@ECPG_INCLUDES@ @CXXFLAGS@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_PROGRAMS=libProdLager.o

includedir=@includedir@/Lager_Vormerkungen

libProdLager_o_SOURCES= $(PGCC_SOURCES) Lager_Vormerkungen.cc Lager.cc LagerPlatz.cc JumboRolle.cc

libProdLager.o: $(libProdLager_o_OBJECTS) $(libProdLager_o_DEPENDENCIES)
	touch $@

PGCC_SOURCES= Lager_Vormerkungen_sql.cc Lager_sql.cc maschinen_geschwindigkeit_sql.cc JumboRolle_sql.cc

BUILT_SOURCES= $(PGCC_SOURCES)

include_HEADERS = Lager_Vormerkungen.hh LagerPlatz.hh \
   maschinen_geschwindigkeit.hh Lager.hh JumboRolle.h CodeError.h

CLEANFILES= *~ $(BUILT_SOURCES) 

