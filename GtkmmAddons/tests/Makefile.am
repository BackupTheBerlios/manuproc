# $Id: Makefile.am,v 1.11 2004/08/30 07:35:07 christof Exp $

SUBDIRS=close_window

AM_CXXFLAGS=-I$(top_srcdir)/searchcombo/src -I$(top_srcdir)/src

EXTRA_PROGRAMS = nsc_test sccont_test optionmenu db_test back_test \
	callbackbuf wfr_test

back_test_SOURCES = back_test.cc db_embed.cc
back_test_LDADD = $(top_srcdir)/src/libGtkmmAddons.a @GTKMM_LIBS@

sccont_test_SOURCES = sccont_test.cc
sccont_test_LDADD = $(top_srcdir)/src/libGtkmmAddons.a @GTKMM_LIBS@

nsc_test_SOURCES = nsc_test.cc
nsc_test_LDADD = $(top_srcdir)/src/libGtkmmAddons.a @GTKMM_LIBS@

wfr_test_SOURCES = wfr_test.cc
wfr_test_LDADD = $(top_srcdir)/src/libGtkmmAddons.a @GTKMM_LIBS@

db_test_SOURCES = db_test.cc db_embed.cc
db_test_LDADD = $(top_srcdir)/src/libGtkmmAddons.a @GTKMM_LIBS@

optionmenu_SOURCES = optionmenu.cc
optionmenu_LDADD = $(top_srcdir)/src/libGtkmmAddons.a @GTKMM_LIBS@

callbackbuf_SOURCES = callbackbuf.cc
callbackbuf_LDADD = $(top_srcdir)/src/libGtkmmAddons.a @GTKMM_LIBS@

INCLUDES = -I$(top_srcdir)/searchcombo/src -I$(top_srcdir)/src @GTKMM_CFLAGS@ 
#AM_LDFLAGS = @GTKMM2_LDFLAGS@

