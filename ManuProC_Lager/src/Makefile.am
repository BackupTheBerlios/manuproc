# generated 2000/1/28 11:31:10 MET by jacek@mimi.
# using glademm V0.5.5
#
# newer (non customized) versions of this file go to Makefile.am_glade



%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

bin_PROGRAMS = fertigwlager

fertigwlager_SOURCES = \
	fertigwlager.cc\
	wlagermain_glade.cc \
	wlagermain.cc 

#glademm_support.cc 

noinst_HEADERS = \
	wlagermain_glade.hh \
	wlagermain.hh 

#glademm_support.hh

BUILT_SOURCES=

fertigwlager_LDADD = @LIBS@ @KOMPONENTEN_LIBS@ @GNOMEMM_LIBS@

CXXFLAGS = -DMABELLA @CXXFLAGS@ @KOMPONENTEN_INCLUDES@ @GNOMEMM_CFLAGS@

LDFLAGS= @LDFLAGS@ @KOMPONENTEN_LDFLAGS@







CLEANFILES=*~ *.pct *.prot $(BUILT_SOURCES) *_p.cc
