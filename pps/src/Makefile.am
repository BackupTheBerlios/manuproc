# generated 2000/11/22 20:43:27 MET by jacek@mimi.
# using glademm V0.5.10
#
# newer (non customized) versions of this file go to Makefile.am_glade

%.cc: %.pgcc ; $(ECPG) -o $*.cc $<

bin_PROGRAMS = auftrag

auftrag_SOURCES = $(BUILT_SOURCES) \
	Rg_Tree.cc \
	OffLief_Tree.cc \
	auftrag_rechnung.cc \
	auftrag_rechnung_glade.cc \
	Lief_Tree.cc \
	OffAuf_Tree.cc \
	OffAuf_RowData.cc \
	aktAufEintrag.cc \
	Auftrag.cc \
	auftrag.cc\
	termsplidial_glade.cc \
	termsplidial.cc \
	auftragliste_glade.cc \
	auftragliste.cc \
	auftrag_main_glade.cc \
	auftrag_main.cc \
	MainTree.cc \
	myRow.cc \
	auftragentry.cc \
	auftrag_bearbeiten.cc \
	auftrag_bearbeiten_glade.cc \
	auftrag_lieferschein.cc \
	auftrag_lieferschein_glade.cc


noinst_HEADERS = \
	termsplidial_glade.hh \
	termsplidial.hh \
	auftragliste_glade.hh \
	auftragliste.hh \
	auftrag_main_glade.hh \
	auftrag_main.hh \
	auftrag_rechnung_glade.hh \
	auftrag_rechnung.hh \
	Rg_Tree.h \
	Rg_TCList.hh \
	OffLief_Tree.h \
	OffLief_TCList.hh

BUILT_SOURCES=auftragentry_sql.cc Auftrag_sql.cc auftrag_bearbeiten_sql.cc\
	auftrag_liefer_sql.cc OffAuf_TCList_sql.cc\
	OffAuf_RowData_sql.cc OffLief_Tree_sql.cc \
	auftrag_rechnung_sql.cc

CXXFLAGS += @CXXFLAGS@ @KOMPONENTEN_INCLUDES@ @GNOMEMM_CFLAGS@ 

LDFLAGS= @LDFLAGS@ @KOMPONENTEN_LDFLAGS@ 

auftrag_LDADD =  @LIBS@ @KOMPONENTEN_LIBS@ @GNOMEMM_LIBS@

