# $Id: Makefile.am,v 1.39 2006/05/17 08:47:48 christof Exp $
AM_CXXFLAGS= @COMMONGTK2_INCLUDES@ @COMMONXX_INCLUDES@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

include_HEADERS = logwin.h EingabeBox.hh \
	RowDataBase.h TreeRow.h treebase_data.h \
	SimpleTreeModel.h SimpleTreeStore.h \
	bool_CheckMenuItem.hh char_Pixmap.hh \
	FlashingPixmap.hh LabelSpin.hh MyWindow.hh MyScale.hh \
	TreeViewUtility.h bool_properties.hh bool_ImageButton.hh \
	ModelWidgetConnection.h string_Label.hh RefPtr_Pixmap.hh \
	Association.h TreeModelDebug.h \
	SearchComboContent.hh int_ImageButton.hh \
	IntStringBox_gui.hh
	
noinst_HEADERS=SimpleTree.hh IntStringBox.hh

lib_LIBRARIES= libManuProC_Widgets.a
EXTRA_PROGRAMS=dummy

libManuProC_Widgets_a_SOURCES=EingabeBox.cc \
		bool_CheckButton.cc \
		int_SpinButton.cc string_Entry.cc \
		SimpleTreeModel.cc SimpleTreeStore.cc \
		GType_cH_EntryValue.cc SimpleTree.cc \
		bool_CheckMenuItem.cc char_Pixmap.cc \
		FlashingPixmap.cc LabelSpin.cc RefPtr_Pixmap.cc \
		MyWindow.cc MyScale.cc logwin.cc TreeViewUtility.cc \
		bool_properties.cc bool_ImageButton.cc string_Label.cc \
		Association.cc Event.cc TooltipView.cc \
		SimpleTreeStore_test.cc CellRendererSimpleTree.cc \
		TreeModelDebug.cc int_ImageButton.cc \
		IntStringBox_gui.cc IntStringBox.cc IntStringBox_glade.cc \
		IntStringPopup_glade.cc datewin.cc datewin_glade.cc \
		datewin_popup.cc datewin_popup_glade.cc BasicExcel.cpp

BUILT_SOURCES=datewin.hh logwin.hh IntStringBox_sql.cc

# alias links
datewin.hh: datewin.h
	ln -sf datewin.h datewin.hh
logwin.hh: logwin.h
	ln -sf logwin.h logwin.hh

CLEANFILES= *~ $(BUILT_SOURCES)
