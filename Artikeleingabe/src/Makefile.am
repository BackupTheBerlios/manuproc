# generated 2001/9/18 9:40:08 CEST by thoma@Tiger.
# using glademm V0.6.2_cvs
#
# newer (non customized) versions of this file go to Makefile.am_new
bin_PROGRAMS = artikeleingabe artikel_shell
# artikel_delete artikel_create	artikel_bestellenbei

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

BUILT_SOURCES = Artikeleingabe_sql.cc

artikeleingabe_SOURCES = \
   $(BUILT_SOURCES) \
	artikeleingabe.cc\
	Artikeleingabe_glade.cc \
	Artikeleingabe.cc 

artikel_shell_SOURCES = artikel_shell.cc

noinst_HEADERS = \
	Artikeleingabe_glade.hh \
	Artikeleingabe.hh 

CXXFLAGS = @CXXFLAGS@ @GNOMEMM_CFLAGS@ @KOMPONENTEN_INCLUDES@
LDFLAGS= @KOMPONENTEN_LDFLAGS@ @LDFLAGS@ 

artikeleingabe_LDADD = @LIBS@ @GNOMEMM_LIBS@ @KOMPONENTEN_LIBS@
artikel_shell_LDADD = @LIBS@ @KOMPONENTEN_LIBS@

#artikel_delete: artikel_shell
#	ln -s $< $@
#artikel_create: artikel_shell
#	ln -s $< $@
#artikel_bestellenbei: artikel_shell
#	ln -s $< $@
