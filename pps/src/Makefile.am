# generated 2000/11/22 20:43:27 MET by jacek@mimi.
# using glademm V0.5.10
#
# newer (non customized) versions of this file go to Makefile.am_glade

%.cc: %.pgcc ; $(ECPG) -o $*.cc $<

bin_PROGRAMS = pps

pps_SOURCES = $(BUILT_SOURCES) \
	AuftragEntry2.cc \
	AuftragFull2.cc \
	Lief_Tree.cc \
	MainTree.cc \
	OffAuf_Tree.cc \
	OffLief_Tree.cc \
	Rg_Tree.cc \
	aktAufEintrag.cc \
	auftrag_bearbeiten.cc \
	auftrag_bearbeiten_glade.cc \
	auftrag_lieferschein.cc \
	auftrag_lieferschein_glade.cc \
	auftrag_main.cc \
	auftrag_main_glade.cc \
	auftrag_rechnung.cc \
	auftrag_rechnung_glade.cc \
	auftragliste.cc \
	auftragliste_glade.cc \
	myRow.cc \
	pps.cc\
	termsplidial.cc \
	termsplidial_glade.cc 

noinst_HEADERS = \
	OffLief_TCList.hh \
	OffLief_Tree.h \
	Rg_TCList.hh \
	Rg_Tree.h \
	auftrag_main.hh \
	auftrag_main_glade.hh \
	auftrag_rechnung.hh \
	auftrag_rechnung_glade.hh \
	auftragliste.hh \
	auftragliste_glade.hh \
	termsplidial.hh \
	termsplidial_glade.hh 

BUILT_SOURCES= \
	AuftragEntry_sql.cc \
	OffAuf_RowData_sql.cc \
	OffAuf_TCList_sql.cc \
	OffLief_Tree_sql.cc \
	auftrag_bearbeiten_sql.cc \
	auftrag_liefer_sql.cc \
	auftrag_rechnung_sql.cc

CXXFLAGS = @CXXFLAGS@ @KOMPONENTEN_INCLUDES@ @GNOMEMM_CFLAGS@ 

LDFLAGS= @LDFLAGS@ @KOMPONENTEN_LDFLAGS@ 

pps_LDADD =  @LIBS@ @KOMPONENTEN_LIBS@ @GNOMEMM_LIBS@

