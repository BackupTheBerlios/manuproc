# $Id: Makefile.am,v 1.3 2002/05/27 11:51:29 malte Exp $

CXXFLAGS=@CXXFLAGS@ @COMMONXX_INCLUDES@
LDFLAGS=@LDFLAGS@  @COMMONXX_LDFLAGS@
LDADD=@COMMONXX_LIBS@

EXTRA_PROGRAMS=grafik_auftragsbaum

grafik_auftragsbaum_SOURCES=grafik_auftragsbaum.cc

CLEANFILES= *~ $(LINKS) $(EXTRA_PROGRAMS)

$(LINKS):
	ln -sf ../../Basic/examples/$@ .
