CXXFLAGS=@CXXFLAGS@ @ECPG_INCLUDES@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_PROGRAMS=libKunde.o

includedir=@includedir@/Kunde

libKunde_o_SOURCES=$(BUILT_SOURCES) Kunde.cc Person.cc KontaktListe.cc \
		Adresse.cc Telefon.cc

libKunde.o: $(libKunde_o_OBJECTS) $(libKunde_o_DEPENDENCIES)
	touch $@

BUILT_SOURCES=Telefon_sql.cc Kunde_sql.cc Person_sql.cc  Adresse_sql.cc \
   Anrede_sql.cc PreisListe_sql.cc LandesListe_sql.cc

include_HEADERS=Kunde.h Telefon.h Person.h Adresse.h Anrede.h PreisListe.h \
      LandesListe.h KontaktListe.h

CLEANFILES= *~ *.pct *.prot $(BUILT_SOURCES)

