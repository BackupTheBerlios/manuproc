# $Id: Makefile.am,v 1.1 2001/04/23 08:36:50 christof Exp $
CXXFLAGS=@CXXFLAGS@ @GTKMM_CFLAGS@ \
		@COMMONGTK_INCLUDES@ @COMMONXX_INCLUDES@ @ECPG_INCLUDES@ 

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

include_HEADERS = datewin.h logwin.h\
	EingabeBox.hh ProzessBox.hh \
	ArtikelBox.hh datewin_glade.hh \
	KundenBox.hh KundenBox_glade.hh \
	SearchComboContent.hh SearchCombo.hh \
	glademm_support.hh  \
	rowdata.h tclistrowdata.h \
	tclistnode.h tclistleaf.h treebase.h

lib_LIBRARIES= libKomponenten.a

libKomponenten_a_SOURCES=datewin.cc \
		ArtikelBox.cc \
		logwin.cc EingabeBox.cc \
		datewin_glade.cc KundenBox_glade.cc \
		glademm_support.cc $(BUILT_SOURCES_L) \
		tclistrowdata.cc \
		tclistnode.cc tclistleaf.cc treebase.cc

BUILT_SOURCES_L= ArtikelBox_sql.cc ProzessBox.cc KundenBox.cc 
BUILT_SOURCES=datewin.hh $(BUILT_SOURCES_L)

# for datewin_glade.cc
datewin.hh: datewin.h
	ln -sf datewin.h datewin.hh

install-data-hook:
	cd $(includedir) ; chmod g+w $(include_HEADERS)
	cd $(includedir) ; rm -f artikel_box_win.hh rohartikel_win.hh \
		rohartikel_win_glade.hh
