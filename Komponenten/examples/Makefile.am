SUBDIRS=artikelbox eingabe tree_simplest MVC

EXTRA_PROGRAMS=Handles Event

Handles_SOURCES=Handles.cc
Event_SOURCES=Event.cc

AM_CXXFLAGS = @KOMPONENTEN2_INCLUDES@ 
Handles_LDADD = @LIBS@ @KOMPONENTEN2_LIBS@
Event_LDADD = @LIBS@ @KOMPONENTEN2_LIBS@
AM_LDFLAGS= @KOMPONENTEN2_LDFLAGS@

LINKS=Handles.cc Event.cc

BUILT_SOURCES=$(LINKS)

$(LINKS):
	ln -sf ../Basic/examples/$@ .

