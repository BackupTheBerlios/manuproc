# $Id: Makefile.am,v 1.17 2002/09/18 07:56:01 christof Exp $
CXXFLAGS=@CXXFLAGS@  \
		@COMMONGTK_INCLUDES@ @COMMONXX_INCLUDES@  

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

LINKS= EingabeBox.cc EingabeBox.hh datewin.cc datewin.h \
	datewin_glade.cc datewin_glade.hh logwin.cc logwin.h \
	rowdata.h tclistleaf.cc tclistleaf.h tclistnode.cc tclistnode.h \
	tclistrowdata.cc tclistrowdata.h treebase.cc treebase.h \
	treebase_data.h MVC_int_Widget.hh MVC_int_Widget.cc \
	MVC_string_Widget.hh MVC_string_Widget.cc \
	MVC_bool_Widget.hh MVC_bool_Widget.cc

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
   Optionmenu_Warengruppe.hh PPixmap.hh \
   Optionmenu_Einheit.hh PreisBox.hh WTelefon.hh \
   IntStringBox.hh IntStringBox_gui.hh PersonenBox.hh \
   LandesBox.hh KundenBox2.hh ZahlungsArtBox.hh \
   MVC_int_Widget.hh MVC_string_Widget.hh MVC_bool_Widget.hh

lib_LIBRARIES= libKomponenten.a

libKomponenten_a_SOURCES=datewin.cc \
		ArtikelBox.cc \
		logwin.cc EingabeBox.cc \
		datewin_glade.cc KundenBox_glade.cc \
		$(BUILT_SOURCES_L) \
		tclistrowdata.cc \
		tclistnode.cc tclistleaf.cc treebase.cc \
      Artikel_Bestellen.cc \
      PPixmap.cc \
      Optionmenu_Einheit.cc \
      Optionmenu_Instanz.cc \
      WTelefon.cc \
      WTelefon_glade.cc \
      IntStringBox_gui.cc \
      IntStringBox.cc \
      PersonenBox.cc \
      PreisBox.cc \
      WPreis.cc \
      LandesBox.cc \
      KundenBox2.cc \
      ZahlungsArtBox.cc \
      MVC_int_Widget.cc MVC_string_Widget.cc MVC_bool_Widget.cc

BUILT_SOURCES_L= ArtikelBox_sql.cc ProzessBox_sql.cc KundenBox_sql.cc $(LINKS) \
   WWaehrung_sql.cc WAuftragStatus_sql.cc \
   Optionmenu_Warengruppe_sql.cc IntStringBox_sql.cc AB_Breitensuche_sql.cc

BUILT_SOURCES=datewin.hh SimpleTree.hh logwin.hh $(BUILT_SOURCES_L) 

# alias links
datewin.hh: datewin.h
	ln -sf datewin.h datewin.hh
logwin.hh: logwin.h
	ln -sf logwin.h logwin.hh
SimpleTree.hh: treebase.h
	ln -sf treebase.h SimpleTree.hh

install-data-hook:
	cd $(includedir) ; chmod g+w $(include_HEADERS)
	cd $(includedir) ; rm -f artikel_box_win.hh rohartikel_win.hh \
		rohartikel_win_glade.hh
	cd $(includedir) ; ln -s treebase.h SimpleTree.hh

CLEANFILES= *~ $(BUILT_SOURCES) $(LINKS)
