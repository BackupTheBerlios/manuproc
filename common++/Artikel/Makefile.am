# $Id: Makefile.am,v 1.18 2003/01/15 15:10:15 christof Exp $

CXXFLAGS=@ECPG_INCLUDES@ @CXXFLAGS@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_LIBRARIES=libArtikel.a

includedir=@includedir@/Artikel

libArtikel_a_SOURCES=Aufmachung.cc Preis.cc ArtikelBase.cc \
	ArtikelMengeSumme.cc \
	$(BUILT_SOURCES_P)

libArtikel.a: $(libArtikel_a_OBJECTS) $(libArtikel_a_DEPENDENCIES)
	touch $@
	
BUILT_SOURCES=$(BUILT_SOURCES_P)

BUILT_SOURCES_P=ArtikelBaum_sql.cc ArtikelBezeichnung.cc \
	Misc.cc Aufmachung_sql.cc Prozesspreis.cc \
	Prozess_sql.cc ArtikelTyp.cc Einheiten.cc ArtikelStamm.cc \
	Artikelpreis_sql.cc ArtikelDispo_sql.cc \
	ArtikelEAN_sql.cc

include_HEADERS=ArtikelBase.h ArtikelBaum.h ArtikelBezeichnung.h \
	Artikelpreis.h Aufmachung.h Misc.h Preis.h Prozess.h Prozesspreis.h \
	Einheiten.h ArtikelTyp.h ArtikelStamm.h \
	ArtikelMengeSumme.h ArtikelDispo.h \
	ArtikelEAN.h

CLEANFILES= *~ *.pct *.prot $(BUILT_SOURCES_P) *_p.cc

