# $Id: Makefile.am,v 1.3 2001/12/27 09:45:58 christof Exp $
CXXFLAGS=@CXXFLAGS@ @COMMONGTK_INCLUDES@ @COMMONXX_INCLUDES@  

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

include_HEADERS = datewin.h logwin.h\
	EingabeBox.hh datewin_glade.hh \
	rowdata.h tclistrowdata.h treebase_data.h \
	tclistnode.h tclistleaf.h treebase.h

noinst_HEADERS=SimpleTree.hh

lib_LIBRARIES= libManuProC_Widgets.a

libManuProC_Widgets_a_SOURCES=datewin.cc \
		logwin.cc EingabeBox.cc \
		datewin_glade.cc tclistrowdata.cc \
		tclistnode.cc tclistleaf.cc treebase.cc

BUILT_SOURCES=datewin.hh SimpleTree.hh

# for datewin_glade.cc
datewin.hh: datewin.h
	ln -sf datewin.h datewin.hh
	
SimpleTree.hh: treebase.h
	ln -sf treebase.h SimpleTree.hh

install-data-hook:
	cd $(includedir) ; ln -sf treebase.h SimpleTree.hh

CLEANFILES= *~ $(BUILT_SOURCES)
