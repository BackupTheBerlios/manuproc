# $Id: Makefile.am,v 1.27 2003/11/26 12:13:32 jacek Exp $

ENDUSER=-DPETIG

#AM_CXXFLAGS=-D MABELLA_LAGERHACK -I/home/share/include @CXXFLAGS@ @ECPG_INCLUDES@
AM_CXXFLAGS=-I/home/share/include @CXXFLAGS@ @ECPG_INCLUDES@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_LIBRARIES=libAuftrag.a

includedir=@includedir@/Auftrag

libAuftrag_a_SOURCES=AufEintrag.cc AufEintragBase.cc AuftragBase.cc \
	sqlAuftragSelector.cc Auftrag.cc $(BUILT_SOURCES_P) AuftragsBaum.cc \
   AufEintragZuMengenAenderung.cc AufEintragZu.cc VerfuegbareMenge.cc \
   ppsInstanzReparatur.cc AufEintrag_ro.cc AufEintrag_loops.cc \
   AufEintrag_Lager.cc AufEintrag_Produktion.cc AufEintrag_Menge.cc \
   delayed_reclaim.cc

libAuftrag.a: $(libAuftrag_a_OBJECTS) $(libAuftrag_a_DEPENDENCIES) Makefile
	(for i in $(libAuftrag_a_OBJECTS) ; do echo Auftrag/$$i ; done ) >$@

BUILT_SOURCES_P= AuftragBase_sql.cc selAufEntry_sql.cc\
		Auftrag_sql.cc AuftragFull_sql.cc 
		
BUILT_SOURCES=$(BUILT_SOURCES_P)

include_HEADERS=AufEintrag.h AuftragBase.h \
	sqlAuftragSelector.h AufEintragBase.h selFullAufEntry.h \
	auftrag_status.h auftrag_enums.h AuftragsBaum.h AufEintragZu.h \
   AufEintragZuMengenAenderung.h

CLEANFILES= *~ *.pct *.prot $(BUILT_SOURCES_P) *_p.cc *_pg.cc

