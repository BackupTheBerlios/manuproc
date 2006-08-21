AM_CXXFLAGS=@ECPG_INCLUDES@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_LIBRARIES=libLieferschein.a

includedir=@includedir@/Lieferschein

libLieferschein_a_SOURCES=Lieferschein.cc LieferscheinEntry.cc \
		LieferscheinList.cc LieferscheinVoll.cc $(BUILT_SOURCES) \
      Rechnung.cc RechnungBase.cc

libLieferschein.a: $(libLieferschein_a_OBJECTS) $(libLieferschein_a_DEPENDENCIES) Makefile
	(for i in $(libLieferschein_a_OBJECTS) ; do echo Lieferschein/$$i ; done ) >$@
        
BUILT_SOURCES= LieferscheinList_sql.cc \
	Lieferschein_sql.cc Rechnung_sql.cc RechnungEntry_sql.cc \
   RechnungVoll_sql.cc 

include_HEADERS=LieferscheinBase.h LieferscheinEntry.h Lieferschein.h \
		LieferscheinList.h LieferscheinVoll.h \
		RechnungBase.h Rechnung.h RechnungEntry.h RechnungVoll.h 

CLEANFILES= *~ *.pct *.prot $(BUILT_SOURCES)

