# generated 2001/6/15 9:52:50 CEST by christof@puck.(none)
# using glademm V0.6.2_cvs
#
# newer (non customized) versions of this file go to Makefile.am_new
noinst_PROGRAMS = tree

BUILT_SOURCES=$(LINKS)

LINKS=simplest.hh with_class.hh tree.cc simplest.cc \
	with_class.cc simplest_glade.hh with_class_glade.hh \
	simplest_glade.cc tree.glade with_class_glade.cc

$(LINKS):
	ln -sf ../../Basic/examples/tree_simplest/$@ .

tree_SOURCES = \
	tree.cc\
	with_class_glade.cc \
	with_class.cc \
	simplest_glade.cc \
	simplest.cc

noinst_HEADERS = \
	with_class_glade.hh \
	with_class.hh \
	simplest.hh \
	simplest_glade.hh

CXXFLAGS = @CXXFLAGS@  -I$(top_srcdir)/src @COMMONGTK_INCLUDES@ \
		@COMMONXX_INCLUDES@

tree_LDADD = @LIBS@ $(top_srcdir)/src/libKomponenten.a \
        @COMMONGTK_LIBS@ @COMMONXX_LIBS@

LDFLAGS= @LDFLAGS@ @COMMONXX_LDFLAGS@ @COMMONGTK_LDFLAGS@
