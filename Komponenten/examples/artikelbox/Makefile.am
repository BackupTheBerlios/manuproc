# $Id: Makefile.am,v 1.3 2001/06/21 09:56:40 christof Exp $
CXXFLAGS+=-Wall -g -I$(top_srcdir)/src  \
	@COMMONXX_INCLUDES@ @COMMONGTK_INCLUDES@

LIBS =  $(top_srcdir)/src/libKomponenten.a \
	@COMMONXX_LIBS@ @COMMONGTK_LIBS@ 
	
LDFLAGS = @LDFLAGS@ @COMMONXX_LDFLAGS@ @COMMONGTK_LDFLAGS@

noinst_PROGRAMS = marc

marc_SOURCES = main.cc window1.cc window1_glade.cc

BUILT_SOURCE = 

CLEANFILES=*~ 
