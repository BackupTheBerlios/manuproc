# $Id: Makefile.am,v 1.1 2001/04/23 08:36:49 christof Exp $
CXXFLAGS+=-Wall -g -I$(top_srcdir)/src @GTKMM_CFLAGS@ \
	@COMMONXX_INCLUDES@ @COMMONGTK_INCLUDES@

LIBS =  $(top_srcdir)/src/libKomponenten.a \
	@COMMONXX_LIBS@ @COMMONGTK_LIBS@ @GTKMM_LIBS@ @ECPG_LIBS@
	
LDFLAGS = @LDFLAGS@ @ECPG_LDFLAGS@

noinst_PROGRAMS = marc

marc_SOURCES = main.cc window1.cc window1_glade.cc

BUILT_SOURCE = 

CLEANFILES=*~ 
