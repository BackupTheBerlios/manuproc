# $Id: Makefile.am,v 1.3 2003/04/10 06:31:11 christof Exp $

SUBDIRS=close_window

AM_CXXFLAGS=-I$(top_srcdir)/searchcombo/src -I$(top_srcdir)/src

EXTRA_PROGRAMS = nsc_test sccont_test optionmenu db_test

sccont_test_SOURCES = sccont_test.cc
sccont_test_LDADD = $(top_srcdir)/src/libGtkmmAddons.a

nsc_test_SOURCES = nsc_test.cc
nsc_test_LDADD = $(top_srcdir)/src/libGtkmmAddons.a

db_test_SOURCES = db_test.cc db_embed.cc
db_test_LDADD = $(top_srcdir)/src/libGtkmmAddons.a

optionmenu_SOURCES = optionmenu.cc
optionmenu_LDADD = $(top_srcdir)/src/libGtkmmAddons.a

INCLUDES = -I$(top_srcdir)/searchcombo/src -I$(top_srcdir)/src @GTKMM_CFLAGS@ 
AM_LDFLAGS = @GTKMM_LIBS@

