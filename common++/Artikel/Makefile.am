# $Id: Makefile.am,v 1.24 2003/07/08 08:46:53 christof Exp $

AM_CXXFLAGS=@ECPG_INCLUDES@ @CXXFLAGS@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_LIBRARIES=libArtikel.a

includedir=@includedir@/Artikel

libArtikel_a_SOURCES=Aufmachung.cc Preis.cc ArtikelBase.cc Artikelpreis.cc \
	ArtikelMengeSumme.cc ArtikelBaum_new_del.cc ArtikelBaum.cc \
	$(BUILT_SOURCES_P)

libArtikel.a: $(libArtikel_a_OBJECTS) $(libArtikel_a_DEPENDENCIES) Makefile
	(for i in $(libArtikel_a_OBJECTS) ; do echo Artikel/$$i ; done ) >$@
	
BUILT_SOURCES=$(BUILT_SOURCES_P)

BUILT_SOURCES_P=ArtikelBezeichnung.cc \
	Misc.cc Aufmachung_sql.cc Prozesspreis.cc \
	Prozess_sql.cc ArtikelTyp.cc Einheiten.cc ArtikelStamm.cc \
	ArtikelDispo_sql.cc \
	ArtikelEAN_sql.cc

include_HEADERS=ArtikelBase.h ArtikelBaum.h ArtikelBezeichnung.h \
	Artikelpreis.h Aufmachung.h Misc.h Preis.h Prozess.h Prozesspreis.h \
	Einheiten.h ArtikelTyp.h ArtikelStamm.h \
	ArtikelMengeSumme.h ArtikelDispo.h \
	ArtikelEAN.h

CLEANFILES= *~ *.pct *.prot $(BUILT_SOURCES_P) *_p.cc

