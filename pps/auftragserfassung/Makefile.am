# generated 2001/6/26 15:06:49 CEST by thoma@Tiger.
# using glademm V0.6.2_cvs
#
# newer (non customized) versions of this file go to Makefile.am_new
noinst_PROGRAMS = auftragserfassung_neu produziert planung

#%.cc: %.pgcc ; @ECPG@ -o $*.cc $<


auftragserfassung_neu_SOURCES = auftragserfassung_neu.cc

produziert_SOURCES=produziert.cc

planung_SOURCES=planung.cc

noinst_HEADERS = auftragserfassung_neu.hh

CXXFLAGS = @CXXFLAGS@ @KOMPONENTEN_INCLUDES@

LDFLAGS= @LDFLAGS@ @KOMPONENTEN_LDFLAGS@ 
#-lefence

auftragserfassung_neu_LDADD = @LIBS@ @KOMPONENTEN_LIBS@
produziert_LDADD = @LIBS@ @KOMPONENTEN_LIBS@
planung_LDADD = @LIBS@ @KOMPONENTEN_LIBS@
