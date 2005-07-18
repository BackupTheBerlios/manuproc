# generated 2000/11/22 20:43:27 MET by jacek@mimi.
# using glademm V0.5.10
#
# newer (non customized) versions of this file go to Makefile.am_glade

%.cc: %.pgcc ; $(ECPG) -o $*.cc $<

bin_PROGRAMS = auftrag_drucken

BUILT_SOURCES = 

auftrag_drucken_SOURCES = \
   $(BUILT_SOURCES) \
   auftrag_drucken.cc \
   drucken.cc \
   drucken_class.cc


noinst_HEADERS = \
   drucken.hh \
   drucken_class.hh

AM_CXXFLAGS = -DUSE_POSTGRES @KOMPONENTEN2_INCLUDES@ -DPRINT_LOGO

AM_LDFLAGS= @KOMPONENTEN2_LDFLAGS@ @ECPG_LDFLAGS@ 

LDADD = @LIBS@ @COMMONXX_LIBS@ @COMMONGTK2_ONLY@ @SIGC_LIBS@
	
