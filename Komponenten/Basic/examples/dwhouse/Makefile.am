# generated 2001/3/2 15:58:56 CET by jacek@mimi.
# using glademm V0.5_11f_cvs
#
# newer (non customized) versions of this file go to Makefile.am_new
noinst_PROGRAMS = main

main_SOURCES = \
	MyNode.cc \
	main.cc\
	MyTreeBase.cc \
	window1_glade.cc \
	window1.cc 

noinst_HEADERS = \
	window1_glade.hh window1.hh \
	MyNode.h MyRowData.hh MyTreeBase.hh MyValue.h

CXXFLAGS = @CXXFLAGS@ -I$(top_srcdir)/src \
	@COMMONGTK2_INCLUDES@ @COMMONXX_INCLUDES@

main_LDADD = @LIBS@ $(top_srcdir)/src/libManuProC_Widgets.a \
	@COMMONGTK2_LIBS@ @COMMONXX_LIBS@

LDFLAGS= @LDFLAGS@ @COMMONXX_LDFLAGS@ @COMMONGTK2_LDFLAGS@
