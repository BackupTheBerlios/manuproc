# $Id: Makefile.am,v 1.1 2002/02/08 21:51:26 christof Exp $

CXXFLAGS=-s @ECPG_INCLUDES@ @CXXFLAGS@


LDADD=../libcommon++.a @ECPG_LIBS@ @ECPG_LDFLAGS@

noinst_PROGRAMS= Instanzen

includedir=@includedir@/Programme

Instanzen_SOURCES = Instanzen.cc


CLEANFILES= *~ 

