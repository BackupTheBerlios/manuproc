CXXFLAGS=@CXXFLAGS@ @ECPG_INCLUDES@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_LIBRARIES=libKunde.a

includedir=@includedir@/Kunde

libKunde_a_SOURCES=$(BUILT_SOURCES) Kunde.cc Person.cc KontaktListe.cc \
		Telefon.cc Zahlungsart.cc

#Adresse.cc  - das gibt es doch schon in Adresse_sql.cc

libKunde.a: $(libKunde_a_OBJECTS) $(libKunde_a_DEPENDENCIES)
	touch $@

BUILT_SOURCES=Telefon_sql.cc Kunde_sql.cc Person_sql.cc  Adresse_sql.cc \
   Anrede_sql.cc PreisListe_sql.cc LandesListe_sql.cc Zahlungsart_sql.cc \
	PreisListeFull_sql.cc

include_HEADERS=Kunde.h Telefon.h Person.h Adresse.h Anrede.h PreisListe.h \
      LandesListe.h KontaktListe.h Zahlungsart.h PreisListeFull.h

CLEANFILES= *~ *.pct *.prot $(BUILT_SOURCES)

