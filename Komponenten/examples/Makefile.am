SUBDIRS=artikelbox eingabe tree_simplest MVC newtree

EXTRA_PROGRAMS=Handles Event owntreemodel test_sts

Handles_SOURCES=Handles.cc
Event_SOURCES=Event.cc
owntreemodel_SOURCES=owntreemodel.cc
test_sts_SOURCES=test_sts.cc

AM_CXXFLAGS = @KOMPONENTEN2_INCLUDES@ 
Handles_LDADD = @LIBS@ @KOMPONENTEN2_LIBS@
Event_LDADD = @LIBS@ @KOMPONENTEN2_LIBS@
owntreemodel_LDADD = @LIBS@ @GTKMM2_LIBS@ @COMMONXX_LIBS@
test_sts_LDADD = @LIBS@ @KOMPONENTEN2_LIBS@
AM_LDFLAGS= @KOMPONENTEN2_LDFLAGS@

LINKS=Handles.cc Event.cc test_sts.cc

BUILT_SOURCES=$(LINKS)

$(LINKS):
	ln -sf ../Basic/examples/$@ .

