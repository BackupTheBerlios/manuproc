# $Id: Makefile.am,v 1.5 2004/04/29 14:24:40 christof Exp $

noinst_PROGRAMS = gsc_test db_test

INCLUDES = -I../src @GTK_CFLAGS@

#LDFLAGS = @GTK_LDFLAGS@

LIBS = $(top_srcdir)/src/libGtkmmAddonsWidgets.a @GTK_LIBS@

gsc_test_DEPENDENCIES = $(top_srcdir)/src/libGtkmmAddonsWidgets.a

gsc_test_SOURCES = gsc_test.c

db_test_DEPENDENCIES = $(top_srcdir)/src/libGtkmmAddonsWidgets.a

db_test_SOURCES = db_test.c

