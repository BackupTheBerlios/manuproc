# $Id: Makefile.am,v 1.9 2002/04/19 06:23:22 christof Exp $

CXXFLAGS=@ECPG_INCLUDES@ @CXXFLAGS@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_PROGRAMS=libArtikel.o

includedir=@includedir@/Artikel

libArtikel_o_SOURCES=Aufmachung.cc Preis.cc \
	ArtikelMengeSumme.cc ArtikelBaum.cc \
	$(BUILT_SOURCES_P)

libArtikel.o: $(libArtikel_o_OBJECTS) $(libArtikel_o_DEPENDENCIES)
	touch $@
	
BUILT_SOURCES=$(BUILT_SOURCES_P)

BUILT_SOURCES_P=ArtikelBaum_sql.cc ArtikelBezeichnung.cc \
	Misc.cc Aufmachung_sql.cc Prozesspreis.cc \
	Prozess_sql.cc ArtikelTyp.cc Einheiten.cc ArtikelStamm.cc \
	Artikelpreis_sql.cc ArtikelDispo_sql.cc

include_HEADERS=ArtikelBase.h ArtikelBaum.h ArtikelBezeichnung.h \
	Artikelpreis.h Aufmachung.h Misc.h Preis.h Prozess.h Prozesspreis.h \
	Einheiten.h ArtikelTyp.h ArtikelStamm.h \
	ArtikelMengeSumme.h ArtikelDispo.h

CLEANFILES= *~ *.pct *.prot $(BUILT_SOURCES_P) *_p.cc

