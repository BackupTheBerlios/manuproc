# generated 2002/7/5 22:59:43 CEST by christof@puck.petig-baender.de
# using glademm V1.1.0d_cvs
#
# newer (non customized) versions of this file go to Makefile.am_new
EXTRA_PROGRAMS = mvc

mvc_SOURCES = \
	mvc.cc\
	window1_glade.cc \
	window1.cc \
	pixmaps.cc

noinst_HEADERS = \
	window1_glade.hh \
	window1.hh 
	
#window1_glade.hh: mvc.glade 
#	glade-- --baseclass mvc.glade
#	rm `cat ../UNNEEDED_FILES `

AM_CXXFLAGS = @KOMPONENTEN_INCLUDES@

mvc_LDADD = @LIBS@ @KOMPONENTEN_LIBS@

AM_LDFLAGS= @KOMPONENTEN_LDFLAGS@
