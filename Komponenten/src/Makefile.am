# $Id: Makefile.am,v 1.3 2001/06/21 08:13:31 christof Exp $
CXXFLAGS=@CXXFLAGS@  \
		@COMMONGTK_INCLUDES@ @COMMONXX_INCLUDES@  

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

LINKS= EingabeBox.cc EingabeBox.hh datewin.cc datewin.h \
	datewin_glade.cc datewin_glade.hh logwin.cc logwin.h \
	rowdata.h tclistleaf.cc tclistleaf.h tclistnode.cc tclistnode.h \
	tclistrowdata.cc tclistrowdata.h treebase.cc treebase.h \
	treebase_data.h

$(LINKS):
	ln -sf ../Basic/src/$@ .

include_HEADERS = datewin.h logwin.h\
	EingabeBox.hh ProzessBox.hh \
	ArtikelBox.hh datewin_glade.hh \
	KundenBox.hh KundenBox_glade.hh \
	SearchComboContent.hh SearchCombo.hh \
	rowdata.h tclistrowdata.h \
	tclistnode.h tclistleaf.h treebase.h

lib_LIBRARIES= libKomponenten.a

libKomponenten_a_SOURCES=datewin.cc \
		ArtikelBox.cc \
		logwin.cc EingabeBox.cc \
		datewin_glade.cc KundenBox_glade.cc \
		$(BUILT_SOURCES_L) \
		tclistrowdata.cc \
		tclistnode.cc tclistleaf.cc treebase.cc

BUILT_SOURCES_L= ArtikelBox_sql.cc ProzessBox.cc KundenBox.cc $(LINKS)
BUILT_SOURCES=datewin.hh SimpleTree.hh $(BUILT_SOURCES_L)

# for datewin_glade.cc
datewin.hh: datewin.h
	ln -sf datewin.h datewin.hh
	
SimpleTree.hh: treebase.h
	ln -sf treebase.h SimpleTree.hh

install-data-hook:
	cd $(includedir) ; chmod g+w $(include_HEADERS)
	cd $(includedir) ; rm -f artikel_box_win.hh rohartikel_win.hh \
		rohartikel_win_glade.hh
	cd $(includedir) ; ln -s treebase.h SimpleTree.hh

CLEANFILES= *~ $(BUILT_SOURCES)
