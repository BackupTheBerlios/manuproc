# $Id: Makefile.am,v 1.2 2003/01/10 17:12:08 christof Exp $

CLEANFILES = *~

GTKMMTOOLS=$(HOME)/sourceforge/gtkmm-2.0/tools

INCLUDES = -I. -I$(top_srcdir)/searchcombo/src @GTKMM_CFLAGS@ 

lib_LIBRARIES = libGtkmmAddons.a

include_HEADERS = Gtk_OStream.h Message.h SearchCombo.h \
		SearchComboContent.h gtksearchcombo.h \
		TeX.h SelectMatching.h

noinst_HEADERS = green_minus.xpm red_plus.xpm

libGtkmmAddons_a_SOURCES = Gtk_OStream_label.cc \
		 Gtk_OStream_optionmenu.cc Gtk_OStream_stream.cc \
		 SearchCombo.cc TeX.cc Gtk_OStream.cc \
		 gtksearchcombo.c Gtk_OStream_TreeView.cc

BUILT_SOURCES=SearchCombo.cc SearchCombo.h gtksearchcombo.c gtksearchcombo.h \
	gtkmm

EXTRA_DIST=SearchCombo.gen_h private/SearchCombo_p.h

gtkmm:
	ln -sf . gtkmm

SearchCombo.cc SearchCombo.h: gtkmm SearchCombo.hg SearchCombo.ccg
	if [ ! -d private ] ; then mkdir private ; fi
	echo perl -I$(GTKMMTOOLS)/pm $(GTKMMTOOLS)/gtkmmproc -I $(GTKMMTOOLS)/m4 --defs . SearchCombo . .

gtksearchcombo.h: $(top_srcdir)/searchcombo/src/gtksearchcombo.h
	ln -s $(top_srcdir)/searchcombo/src/gtksearchcombo.h .

gtksearchcombo.c: $(top_srcdir)/searchcombo/src/gtksearchcombo.c
	ln -s $(top_srcdir)/searchcombo/src/gtksearchcombo.c .

