# generated 2002/11/7 16:33:48 CET by christof@puck.petig-baender.de
# using glademm V1.1.2a_cvs
#
# newer (non customized) versions of this file go to Makefile.am_new
EXTRA_PROGRAMS = newtree

BUILT_SOURCES=$(LINKS)

LINKS=newtree.cc window1.cc window1_glade.cc window1_glade.hh window1.hh

$(LINKS):
	ln -sf ../../Basic/examples/newtree/$@ .

newtree_SOURCES = \
	newtree.cc\
	window1_glade.cc \
	window1.cc 

noinst_HEADERS = \
	window1_glade.hh \
	window1.hh 

AM_CXXFLAGS = @KOMPONENTEN2_INCLUDES@

newtree_LDADD = @LIBS@ @KOMPONENTEN2_LIBS@

AM_LDFLAGS= @KOMPONENTEN2_LDFLAGS@
