# $Id: Makefile.am,v 1.20 2002/12/16 08:29:33 christof Exp $
CXXFLAGS=@CXXFLAGS@ @COMMONGTK2_INCLUDES@ @COMMONXX_INCLUDES@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

include_HEADERS = logwin.h EingabeBox.hh \
	RowDataBase.h TreeRow.h treebase_data.h \
	SimpleTreeModel.h SimpleTreeStore.h \
	bool_CheckMenuItem.hh char_Pixmap.hh \
	FlashingPixmap.hh LabelSpin.hh MyWindow.hh MyScale.hh \
	TreeViewUtility.h
	
noinst_HEADERS=SimpleTree.hh

lib_LIBRARIES= libManuProC_Widgets.a
EXTRA_PROGRAMS=dummy

libManuProC_Widgets_a_SOURCES=EingabeBox.cc \
		bool_CheckButton.cc \
		int_SpinButton.cc string_Entry.cc \
		SimpleTreeModel.cc SimpleTreeStore.cc \
		GType_cH_EntryValue.cc SimpleTree.cc \
		bool_CheckMenuItem.cc char_Pixmap.cc \
		FlashingPixmap.cc LabelSpin.cc \
		MyWindow.cc MyScale.cc logwin.cc TreeViewUtility.cc

dummy_SOURCES=datewin.cc datewin_glade.cc

BUILT_SOURCES=datewin.hh logwin.hh

# alias links
datewin.hh: datewin.h
	ln -sf datewin.h datewin.hh
logwin.hh: logwin.h
	ln -sf logwin.h logwin.hh

CLEANFILES= *~ $(BUILT_SOURCES)
