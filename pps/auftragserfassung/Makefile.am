# generated 2001/6/26 15:06:49 CEST by thoma@Tiger.
# using glademm V0.6.2_cvs
#
# newer (non customized) versions of this file go to Makefile.am_new
bin_PROGRAMS = auftragserfassung

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<


auftragserfassung_SOURCES = \
	auftragserfassung.cc      

#BUILD_SOURCES = auftragserfassung_sql.cc

noinst_HEADERS = \
	auftragserfassung.hh

CXXFLAGS = @CXXFLAGS@ @KOMPONENTEN_INCLUDES@

LDFLAGS= @LDFLAGS@ @KOMPONENTEN_LDFLAGS@


auftragserfassung_LDADD = @LIBS@ @KOMPONENTEN_LIBS@
