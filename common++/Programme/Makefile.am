# $Id: Makefile.am,v 1.3 2002/04/11 12:01:08 christof Exp $

CXXFLAGS=@CXXFLAGS@ -s @COMMONXX_INCLUDES@ 
LDADD=@COMMONXX_LIBS@ @LIBS@
LDFLAGS=@COMMONXX_LDFLAGS@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_PROGRAMS= Instanzen
EXTRA_PROGRAMS = checkGeliefert

Instanzen_SOURCES = Instanzen.cc
checkGeliefert_SOURCES= checkGeliefert_sql.cc

CLEANFILES= *~ 

BUILT_SOURCES=checkGeliefert_sql.cc
