# $Id: Makefile.am,v 1.5 2002/04/23 20:52:20 christof Exp $
CXXFLAGS=@CXXFLAGS@ @COMMONGTK_INCLUDES@ @COMMONXX_INCLUDES@  

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

include_HEADERS = logwin.h EingabeBox.hh \
	rowdata.h tclistrowdata.h treebase_data.h \
	tclistnode.h tclistleaf.h treebase.h \
	datewin.h datewin_glade.hh
	
noinst_HEADERS=SimpleTree.hh

lib_LIBRARIES= libManuProC_Widgets.a
EXTRA_PROGRAMS=dummy

libManuProC_Widgets_a_SOURCES=logwin.cc EingabeBox.cc \
		tclistrowdata.cc \
		tclistnode.cc tclistleaf.cc treebase.cc

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
