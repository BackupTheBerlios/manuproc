# $Id: Makefile.am,v 1.9 2002/11/22 15:31:05 christof Exp $

ENDUSER=-DPETIG

CXXFLAGS=@ECPG_INCLUDES@ @CXXFLAGS@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_LIBRARIES=libProdLager.a

includedir=@includedir@/Lager

libProdLager_a_SOURCES= $(PGCC_Sources) Lager.cc \
   LagerPlatz.cc JumboRolle.cc JumboLager.cc FertigWaren.cc

libProdLager.a: $(libProdLager_a_OBJECTS) $(libProdLager_a_DEPENDENCIES)
	touch $@

PGCC_Sources= Lager_Dispo_sql.cc \
   maschinen_geschwindigkeit_sql.cc JumboRolle_sql.cc \
   FertigWarenLager_sql.cc FertigWaren_sql.cc JumboLager_sql.cc \
	RohwarenLager_sql.cc

BUILT_SOURCES= $(PGCC_Sources)

include_HEADERS = LagerPlatz.hh Lager_Dispo.h \
   maschinen_geschwindigkeit.hh Lager.h JumboRolle.h CodeError.h \
   FertigWaren.h FertigWarenLager.h JumboLager.h RohwarenLager.h

CLEANFILES= *~ $(BUILT_SOURCES) 

