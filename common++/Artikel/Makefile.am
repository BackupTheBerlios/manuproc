# $Id: Makefile.am,v 1.1 2001/04/23 08:11:58 christof Exp $

CXXFLAGS=@GTKMM_CFLAGS@ @ECPG_INCLUDES@ @CXXFLAGS@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_PROGRAMS=libArtikel.o

includedir=@includedir@/Artikel

libArtikel_o_SOURCES=Aufmachung.cc Preis.cc \
	$(BUILT_SOURCES_P) Artikelpreis.cc
	
BUILT_SOURCES=$(BUILT_SOURCES_P)

BUILT_SOURCES_P=ArtikelBaum.cc ArtikelBezeichnung.cc \
	Misc.cc Aufmachung_sql.cc Prozesspreis.cc \
	Prozess_sql.cc ArtikelTyp.cc Einheiten.cc 

include_HEADERS=ArtikelBase.h ArtikelBaum.h ArtikelBezeichnung.h \
	Artikelpreis.h Aufmachung.h Misc.h Preis.h Prozess.h Prozesspreis.h \
	Einheiten.h ArtikelTyp.h

CLEANFILES= *~ *.pct *.prot $(BUILT_SOURCES_P) *_p.cc

