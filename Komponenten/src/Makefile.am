# $Id: Makefile.am,v 1.8 2001/12/19 10:59:11 christof Exp $
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
	tclistnode.h tclistleaf.h treebase.h \
   WWaehrung.hh WPreis.hh WAuftragStatus.hh \
   Optionmenu_Instanz.hh Artikel_Bestellen.hh \
   Optionmenu_Warengruppe.hh PPixmap.hh

lib_LIBRARIES= libKomponenten.a

libKomponenten_a_SOURCES=datewin.cc \
		ArtikelBox.cc \
		logwin.cc EingabeBox.cc \
		datewin_glade.cc KundenBox_glade.cc \
		$(BUILT_SOURCES_L) \
		tclistrowdata.cc \
		tclistnode.cc tclistleaf.cc treebase.cc \
      Artikel_Bestellen.cc \
      PPixmap.cc


BUILT_SOURCES_L= ArtikelBox_sql.cc ProzessBox.cc KundenBox.cc $(LINKS) \
   WWaehrung_sql.cc WPreis_sql.cc WAuftragStatus_sql.cc \
   Optionmenu_Instanz_sql.cc Optionmenu_Warengruppe_sql.cc

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
