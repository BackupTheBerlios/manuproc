# $Id: Makefile.am,v 1.2 2002/02/28 15:19:29 christof Exp $

CXXFLAGS=@CXXFLAGS@ -s @COMMONXX_INCLUDES@ 
LDADD=@COMMONXX_LIBS@ @LIBS@
LDFLAGS=@COMMONXX_LDFLAGS@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_PROGRAMS= Instanzen
EXTRA_PROGRAMS = checkGeliefert

Instanzen_SOURCES = Instanzen.cc
checkGeliefert_SOURCES= checkGeliefert.cc

CLEANFILES= *~ 

BUILT_SOURCES=checkGeliefert.cc
