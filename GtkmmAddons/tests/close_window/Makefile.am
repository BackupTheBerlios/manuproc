# $Id: Makefile.am,v 1.2 2003/11/03 16:39:54 christof Exp $

EXTRA_PROGRAMS = close_window

close_window_SOURCES = \
	close_window.cc\
	window1_glade.cc \
	window1.cc \
	window2_glade.cc \
	window2.cc 

noinst_HEADERS = \
	window1_glade.hh \
	window1.hh \
	window2_glade.hh \
	window2.hh 

AM_CXXFLAGS = @GTKMM_CFLAGS@

close_window_LDADD = @GTKMM_LIBS@

EXTRA_DIST= close_window.glade
