# $Id: Makefile.am,v 1.16 2003/01/15 15:10:16 christof Exp $

ENDUSER=-DPETIG

CXXFLAGS=-I/home/share/include @CXXFLAGS@ @ECPG_INCLUDES@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_LIBRARIES=libAuftrag.a

includedir=@includedir@/Auftrag

# this gives us free compilation
libAuftrag_a_SOURCES=AufEintrag.cc AufEintragBase.cc AuftragBase.cc \
	sqlAuftragSelector.cc Auftrag.cc $(BUILT_SOURCES_P) AuftragsBaum.cc \
   AufEintragZuMengenAenderung.cc AufEintragZu.cc VerfuegbareMenge.cc

libAuftrag.a: $(libAuftrag_a_OBJECTS) $(libAuftrag_a_DEPENDENCIES)
	touch $@

BUILT_SOURCES_P= AuftragBase_sql.cc AufEintrag_sql.cc selAufEntry_sql.cc\
		AufEintragBase_sql.cc Auftrag_sql.cc AuftragFull_sql.cc \
      AufEintragZu_sql.cc
		
BUILT_SOURCES=$(BUILT_SOURCES_P)

include_HEADERS=AufEintrag.h AuftragBase.h \
	sqlAuftragSelector.h AufEintragBase.h selFullAufEntry.h \
	auftrag_status.h auftrag_enums.h AuftragsBaum.h AufEintragZu.h \
   AufEintragZuMengenAenderung.h

CLEANFILES= *~ *.pct *.prot $(BUILT_SOURCES_P) *_p.cc *_pg.cc

