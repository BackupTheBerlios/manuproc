SUBDIRS=artikelbox eingabe tree_simplest MVC

EXTRA_PROGRAMS=Handles Event owntreemodel

Handles_SOURCES=Handles.cc
Event_SOURCES=Event.cc
owntreemodel_SOURCES=owntreemodel.cc

AM_CXXFLAGS = @KOMPONENTEN2_INCLUDES@ 
Handles_LDADD = @LIBS@ @KOMPONENTEN2_LIBS@
Event_LDADD = @LIBS@ @KOMPONENTEN2_LIBS@
owntreemodel_LDADD = @LIBS@ @GTKMM2_LIBS@ @COMMONXX_LIBS@
AM_LDFLAGS= @KOMPONENTEN2_LDFLAGS@

LINKS=Handles.cc Event.cc

BUILT_SOURCES=$(LINKS)

$(LINKS):
	ln -sf ../Basic/examples/$@ .

