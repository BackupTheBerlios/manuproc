# $Id: Makefile.am,v 1.5 2002/05/06 13:41:23 christof Exp $

ENDUSER=-DPETIG

CXXFLAGS=@ECPG_INCLUDES@ @CXXFLAGS@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_PROGRAMS=libProdLager.o

includedir=@includedir@/Lager_Vormerkungen

libProdLager_o_SOURCES= $(PGCC_SOURCES) Lager_Vormerkungen.cc Lager.cc \
   LagerPlatz.cc JumboRolle.cc JumboLager.cc

libProdLager.o: $(libProdLager_o_OBJECTS) $(libProdLager_o_DEPENDENCIES)
	touch $@

PGCC_SOURCES= Lager_Vormerkungen_sql.cc Lager_Dispo_sql.cc \
   maschinen_geschwindigkeit_sql.cc JumboRolle_sql.cc \
   FertigWarenLager_sql.cc FertigWaren_sql.cc JumboLager_sql.cc RohwarenLager_sql.cc

BUILT_SOURCES= $(PGCC_SOURCES)

include_HEADERS = Lager_Vormerkungen.h LagerPlatz.hh Lager_Dispo.h \
   maschinen_geschwindigkeit.hh Lager.h JumboRolle.h CodeError.h \
   FertigWaren.h FertigWarenLager.h JumboLager.h RohwarenLager.h

CLEANFILES= *~ $(BUILT_SOURCES) 

