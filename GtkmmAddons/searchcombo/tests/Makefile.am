# $Id: Makefile.am,v 1.1 2003/01/06 15:07:32 christof Exp $

noinst_PROGRAMS = gsc_test

INCLUDES = -I../src @GTK_CFLAGS@

#LDFLAGS = @GTK_LDFLAGS@

LIBS = $(top_srcdir)/src/libGtkmmAddonsWidgets.a @GTK_LIBS@

gsc_test_DEPENDENCIES = $(top_srcdir)/src/libGtkmmAddonsWidgets.a

gsc_test_SOURCES = gsc_test.c

