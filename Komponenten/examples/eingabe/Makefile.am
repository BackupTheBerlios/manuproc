# $Id: Makefile.am,v 1.1 2001/04/23 08:36:50 christof Exp $
CXXFLAGS=@CXXFLAGS@ -I$(top_srcdir)/src @GNOMEMM_CFLAGS@ \
	@COMMONGTK_INCLUDES@ @COMMONXX_INCLUDES@

eingabe_LDADD = @LIBS@ $(top_srcdir)/src/libKomponenten.a \
	@COMMONGTK_LIBS@ @COMMONXX_LIBS@ @GNOMEMM_LIBS@

LDFLAGS= @LDFLAGS@ @COMMONXX_LDFLAGS@ @COMMONGTK_LDFLAGS@

noinst_PROGRAMS = eingabe

eingabe_SOURCES = eingabe.cc window1.cc window1_glade.cc

CLEANFILES=*~ 
