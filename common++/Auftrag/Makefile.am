# $Id: Makefile.am,v 1.1 2001/04/23 08:11:58 christof Exp $

ENDUSER=-DPETIG

CXXFLAGS=-I/home/share/include @CXXFLAGS@ @ECPG_INCLUDES@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_PROGRAMS=libAuftrag.o

includedir=@includedir@/Auftrag

libAuftrag_o_SOURCES=AufEintragBase.cc selAufEntry.cc \
	sqlAuftragSelector.cc $(BUILT_SOURCES_P)

BUILT_SOURCES_P=AufEintragBase_sql.cc selAufEntry_sql.cc\
		AufEintragBase2.cc
		
BUILT_SOURCES=$(BUILT_SOURCES_P)

include_HEADERS=AufEintragBase.h AuftragBase.h selAufEntry.h \
	sqlAuftragSelector.h AufEintragBase2.h selFullAufEntry.h \
	auftrag_status.h

CLEANFILES= *~ *.pct *.prot $(BUILT_SOURCES_P) *_p.cc *_pg.cc

