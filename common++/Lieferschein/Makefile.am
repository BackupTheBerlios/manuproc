CXXFLAGS=@CXXFLAGS@ @ECPG_INCLUDES@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_PROGRAMS=libLieferschein.o

includedir=@includedir@/Lieferschein

libLieferschein_o_SOURCES=Lieferschein.cc \
		LieferscheinList.cc LieferscheinVoll.cc $(BUILT_SOURCES)

libLieferschein.o: $(libLieferschein_o_OBJECTS) $(libLieferschein_o_DEPENDENCIES)
	touch $@
        
BUILT_SOURCES=LieferscheinEntry_sql.cc LieferscheinList_sql.cc \
	Lieferschein_sql.cc Rechnung_sql.cc RechnungEntry_sql.cc \
   RechnungVoll_sql.cc Zahlungsart_sql.cc

include_HEADERS=LieferscheinBase.h LieferscheinEntry.h Lieferschein.h \
		LieferscheinList.h LieferscheinVoll.h \
		RechnungBase.h Rechnung.h RechnungEntry.h RechnungVoll.h \
      Zahlungsart.hh

CLEANFILES= *~ *.pct *.prot $(BUILT_SOURCES)

