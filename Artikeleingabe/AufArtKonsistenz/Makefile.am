# generated 2001/9/18 9:40:08 CEST by thoma@Tiger.
# using glademm V0.6.2_cvs
#
# newer (non customized) versions of this file go to Makefile.am_new
bin_PROGRAMS = AufArtKonsistenz_neu auftrags_shell

#%.cc: %.pgcc ; @ECPG@ -o $*.cc $<
#BUILT_SOURCES = Artikeleingabe_sql.cc

AufArtKonsistenz_neu_SOURCES = \
   AufArtKonsistenz_neu.cc

auftrags_shell_SOURCES = \
   auftrags_shell.cc


noinst_HEADERS = \
   AufArtKonsistenz_neu.h

CXXFLAGS = @CXXFLAGS@ @KOMPONENTEN_INCLUDES@
LDFLAGS= @KOMPONENTEN_LDFLAGS@ @LDFLAGS@ 

AufArtKonsistenz_neu_LDADD = @LIBS@ @KOMPONENTEN_LIBS@
auftrags_shell_LDADD = @LIBS@ @KOMPONENTEN_LIBS@
