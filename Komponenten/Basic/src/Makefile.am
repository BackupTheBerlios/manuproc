# $Id: Makefile.am,v 1.9 2002/10/18 10:39:42 christof Exp $
CXXFLAGS=@CXXFLAGS@ @COMMONGTK2_INCLUDES@ @COMMONXX_INCLUDES@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

include_HEADERS = logwin.h EingabeBox.hh \
	RowDataBase.h TreeRow.h treebase_data.h \
	MVC_int_Widget.hh \
	MVC_string_Widget.hh MVC_bool_Widget.hh \
	SimpleTreeModel.h SimpleTreeStore.h
	
noinst_HEADERS=SimpleTree.hh

lib_LIBRARIES= libManuProC_Widgets.a
EXTRA_PROGRAMS=dummy

libManuProC_Widgets_a_SOURCES=EingabeBox.cc \
		MVC_int_Widget.cc MVC_string_Widget.cc MVC_bool_Widget.cc \
		SimpleTreeModel.cc SimpleTreeStore.cc

dummy_SOURCES=datewin.cc datewin_glade.cc

BUILT_SOURCES=datewin.hh SimpleTree.hh logwin.hh

# alias links
datewin.hh: datewin.h
	ln -sf datewin.h datewin.hh
SimpleTree.hh: treebase.h
	ln -sf treebase.h SimpleTree.hh
logwin.hh: logwin.h
	ln -sf logwin.h logwin.hh

install-data-hook:
	cd $(includedir) ; ln -sf treebase.h SimpleTree.hh

CLEANFILES= *~ $(BUILT_SOURCES)
