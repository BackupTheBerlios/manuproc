# generated 2001/3/2 15:58:56 CET by jacek@mimi.
# using glademm V0.5_11f_cvs
#
# newer (non customized) versions of this file go to Makefile.am_new
noinst_PROGRAMS = main

BUILT_SOURCES=$(LINKS)

LINKS=MyLeaf.cc MyLeaf.h MyNode.cc MyNode.h MyRowData.hh MyTreeBase.cc \
	MyTreeBase.hh MyValue.h dwhouse.glade main.cc window1.cc \
	window1.hh window1_glade.cc window1_glade.hh

$(LINKS):
	ln -sf ../../Basic/examples/dwhouse/$@ .

main_SOURCES = \
	MyNode.cc \
	MyLeaf.cc \
	main.cc\
	MyTreeBase.cc \
	window1_glade.cc \
	window1.cc 

noinst_HEADERS = \
	window1_glade.hh \
	window1.hh 

CXXFLAGS = @CXXFLAGS@ -I$(top_srcdir)/src \
	@COMMONGTK_INCLUDES@ @COMMONXX_INCLUDES@

main_LDADD = @LIBS@ $(top_srcdir)/src/libKomponenten.a \
	@COMMONGTK_LIBS@ @COMMONXX_LIBS@

LDFLAGS= @LDFLAGS@ @COMMONXX_LDFLAGS@ @COMMONGTK_LDFLAGS@
