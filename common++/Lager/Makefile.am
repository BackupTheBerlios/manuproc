# $Id: Makefile.am,v 1.13 2003/07/15 13:18:25 jacek Exp $

ENDUSER=-DPETIG

CXXFLAGS=@ECPG_INCLUDES@ @CXXFLAGS@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_LIBRARIES=libProdLager.a

includedir=@includedir@/Lager

libProdLager_a_SOURCES= $(PGCC_Sources) \
	FertigWarenLager.cc \
	FertigWaren.cc \
	Lager.cc RohwarenLager.cc \
   	LagerPlatz.cc JumboRolle.cc JumboLager.cc 


libProdLager.a: $(libProdLager_a_OBJECTS) $(libProdLager_a_DEPENDENCIES) Makefile
	(for i in $(libProdLager_a_OBJECTS) ; do echo Lager/$$i ; done ) >$@

PGCC_Sources= Lager_Dispo_sql.cc \
   maschinen_geschwindigkeit_sql.cc JumboRolle_sql.cc \
   JumboLager_sql.cc \
	RohwarenLager_sql.cc

BUILT_SOURCES= $(PGCC_Sources)

include_HEADERS = LagerPlatz.hh Lager_Dispo.h \
   maschinen_geschwindigkeit.hh Lager.h JumboRolle.h CodeError.h \
   FertigWaren.h FertigWarenLager.h JumboLager.h RohwarenLager.h

CLEANFILES= *~ $(BUILT_SOURCES) 

