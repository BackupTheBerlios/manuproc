# generated 2000/11/22 20:43:27 MET by jacek@mimi.
# using glademm V0.5.10
#
# newer (non customized) versions of this file go to Makefile.am_glade

%.cc: %.pgcc ; $(ECPG) -o $*.cc $<

bin_PROGRAMS = pps

pps_SOURCES = pps.cc \
	remote_command.cc \
	$(BUILT_SOURCES) \
	auftrag_main.cc \
	auftrag_main_glade.cc \
	auftrag_main_instanz.cc \
	auftrag_bearbeiten.cc \
	auftrag_bearbeiten_glade.cc \
	auftrag_lieferschein.cc \
	auftrag_lieferschein_glade.cc \
	lieferscheinliste.cc \
	lieferscheinliste_glade.cc \
	auftrag_rechnung.cc \
	auftrag_rechnung_glade.cc \
	termsplidial.cc \
	termsplidial_glade.cc \
	lieferscheinliste_classes.cc \
	ja_nein_frage.cc \
	ja_nein_frage_glade.cc \
	auftrag_copy.cc \
	auftrag_copy_glade.cc \
	auftrag_provision.cc \
	auftrag_provision_glade.cc \
	buchen_dialog.cc \
	buchen_dialog_glade.cc \
	info_dialog_glade.cc \
	info_dialog.cc \
	petig_we.cc petig_we_glade.cc \
	bestell_plan.cc \
	bestell_plan_glade.cc

#	auftrag_main_lager.cc 


noinst_HEADERS = \
	auftrag_main.hh \
	auftrag_main_glade.hh \
   lieferscheinliste.hh \
   lieferscheinliste_classes.hh \
   lieferscheinliste_glade.hh \
	auftrag_rechnung.hh \
	auftrag_rechnung_glade.hh \
	termsplidial.hh \
	termsplidial_glade.hh \
	ja_nein_frage_glade.hh \
	ja_nein_frage.hh \
	auftrag_copy.hh \
	auftrag_copy_glade.hh \
	auftrag_provision.hh \
	auftrag_provision_glade.hh \
	buchen_dialog.hh \
	buchen_dialog_glade.hh \
	info_dialog_glade.hh \
	info_dialog.hh \
	petig_we.hh petig_we_glade.hh \
	bestell_plan.hh \
	bestell_plan_glade.hh \
	datum_kumul.h

BUILT_SOURCES= \
	auftrag_bearbeiten_sql.cc \
	auftrag_liefer_sql.cc \
	auftrag_rechnung_sql.cc \
	auftrag_main_instanz_sql.cc


AM_CXXFLAGS = @KOMPONENTEN_INCLUDES@ @GNOMEMM_CFLAGS@ 

AM_LDFLAGS= @KOMPONENTEN_LDFLAGS@

pps_LDADD =  @LIBS@ @KOMPONENTEN_LIBS@ @GNOMEMM_LIBS@

CLEANFILES = *~ *_new $(BUILT_SOURCES) *_old

