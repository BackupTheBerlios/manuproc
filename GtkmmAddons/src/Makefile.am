# $Id: Makefile.am,v 1.14 2004/04/29 16:44:50 christof Exp $

CLEANFILES = *~

GTKMMTOOLS=$(HOME)/sourceforge/gtkmm-2.2/tools

INCLUDES = -I. -I$(top_srcdir)/searchcombo/src @GTKMM_CFLAGS@ 

lib_LIBRARIES = libGtkmmAddons.a

include_HEADERS = Gtk_OStream.h Message.h SearchCombo.h \
		SearchComboContent.h gtksearchcombo.h \
		TeX.h SelectMatching.h SigC2SlotEmu.h

libGtkmmAddons_a_SOURCES = Gtk_OStream_label.cc \
		 Gtk_OStream_optionmenu.cc Gtk_OStream_stream.cc \
		 SearchCombo.cc TeX.cc Gtk_OStream.cc \
		 gtksearchcombo.c Gtk_OStream_TreeView.cc \
		 DoubleButton.cc doublebutton.c WinFileReq.cc \
		 ChoiceButton.cc gtkhacks.cc Gtk2TeX.cc

BUILT_SOURCES=SearchCombo.cc SearchCombo.h gtksearchcombo.c gtksearchcombo.h \
	gtkmm DoubleButton.h DoubleButton.cc doublebutton.c doublebutton.h \
	WinFileReq_glade.hh WinFileReq.hh

EXTRA_DIST=SearchCombo.hg SearchCombo.ccg private/SearchCombo_p.h \
	WinFileReq_glade.cc 

gtkmm:
	ln -sf . gtkmm

SearchCombo.cc SearchCombo.h: gtkmm SearchCombo.hg SearchCombo.ccg
	if [ ! -d private ] ; then mkdir private ; fi
	echo perl -I$(GTKMMTOOLS)/pm $(GTKMMTOOLS)/gtkmmproc -I $(GTKMMTOOLS)/m4 --defs . SearchCombo . .
	echo /usr/lib/glibmm-2.4/proc/gmmproc --defs . SearchCombo . .

DoubleButton.cc DoubleButton.h: gtkmm DoubleButton.hg DoubleButton.ccg
	if [ ! -d private ] ; then mkdir private ; fi
	echo perl -I$(GTKMMTOOLS)/pm $(GTKMMTOOLS)/gtkmmproc -I $(GTKMMTOOLS)/m4 --defs . DoubleButton . .
	echo /usr/lib/glibmm-2.4/proc/gmmproc --defs . DoubleButton . .

gtksearchcombo.h gtksearchcombo.c doublebutton.c doublebutton.h \
		: $(top_srcdir)/searchcombo/src/$@
	ln -sf $(top_srcdir)/searchcombo/src/$@ .
