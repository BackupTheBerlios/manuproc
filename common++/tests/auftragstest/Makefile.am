# $Id: Makefile.am,v 1.4 2002/06/20 06:51:35 cvs_christof Exp $

CXXFLAGS=@CXXFLAGS@ @COMMONXX_INCLUDES@
LDFLAGS=@LDFLAGS@  @COMMONXX_LDFLAGS@
LDADD=@COMMONXX_LIBS@

EXTRA_PROGRAMS=grafik_auftragsbaum

grafik_auftragsbaum_SOURCES=grafik_auftragsbaum.cc

CLEANFILES= *~ $(LINKS) $(EXTRA_PROGRAMS)

$(LINKS):
	ln -sf ../../Basic/examples/$@ .
