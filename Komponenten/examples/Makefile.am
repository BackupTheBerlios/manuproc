SUBDIRS=artikelbox eingabe tree_simplest MVC

EXTRA_PROGRAMS=Handles

Handles_SOURCES=Handles.cc

CXXFLAGS = @CXXFLAGS@  @KOMPONENTEN_INCLUDES@ 
Handles_LDADD = @LIBS@ @KOMPONENTEN_LIBS@
LDFLAGS= @LDFLAGS@ @KOMPONENTEN_LDFLAGS@

LINKS=Handles.cc

BUILT_SOURCES=$(LINKS)

$(LINKS):
	ln -sf ../Basic/examples/$@ .

