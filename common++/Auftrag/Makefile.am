# $Id: Makefile.am,v 1.2 2001/05/10 16:31:37 christof Exp $

ENDUSER=-DPETIG

CXXFLAGS=-I/home/share/include @CXXFLAGS@ @ECPG_INCLUDES@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_PROGRAMS=libAuftrag.o

includedir=@includedir@/Auftrag

# this gives us free compilation
libAuftrag_o_SOURCES=AufEintragBase.cc selAufEntry.cc \
	sqlAuftragSelector.cc Auftrag.cc $(BUILT_SOURCES_P) 

libAuftrag.o: $(libAuftrag_o_OBJECTS) $(libAuftrag_o_DEPENDENCIES)
	touch $@

BUILT_SOURCES_P=AufEintragBase_sql.cc selAufEntry_sql.cc\
		AufEintragBase2.cc Auftrag_sql.cc
		
BUILT_SOURCES=$(BUILT_SOURCES_P)

include_HEADERS=AufEintragBase.h AuftragBase.h selAufEntry.h \
	sqlAuftragSelector.h AufEintragBase2.h selFullAufEntry.h \
	auftrag_status.h

CLEANFILES= *~ *.pct *.prot $(BUILT_SOURCES_P) *_p.cc *_pg.cc

