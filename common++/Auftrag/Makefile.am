# $Id: Makefile.am,v 1.7 2002/01/22 09:15:55 christof Exp $

ENDUSER=-DPETIG

CXXFLAGS=-I/home/share/include @CXXFLAGS@ @ECPG_INCLUDES@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_PROGRAMS=libAuftrag.o

includedir=@includedir@/Auftrag

# this gives us free compilation
libAuftrag_o_SOURCES=AufEintragBase.cc \
	sqlAuftragSelector.cc Auftrag.cc $(BUILT_SOURCES_P) AuftragsBaum.cc 

libAuftrag.o: $(libAuftrag_o_OBJECTS) $(libAuftrag_o_DEPENDENCIES)
	touch $@

BUILT_SOURCES_P= AuftragBase_sql.cc AufEintragBase_sql.cc selAufEntry_sql.cc\
		AufEintragBase2.cc Auftrag_sql.cc AuftragFull_sql.cc \
      AuftragsEntryZuordnung_sql.cc
		
BUILT_SOURCES=$(BUILT_SOURCES_P)

include_HEADERS=AufEintragBase.h AuftragBase.h \
	sqlAuftragSelector.h AufEintragBase2.h selFullAufEntry.h \
	auftrag_status.h AuftragsBaum.h AuftragsEntryZuordnung.h

CLEANFILES= *~ *.pct *.prot $(BUILT_SOURCES_P) *_p.cc *_pg.cc

