# $Id: Makefile.am,v 1.5 2002/05/06 13:41:23 christof Exp $

CXXFLAGS=@CXXFLAGS@ -s @COMMONXX_INCLUDES@ 
LDADD=@COMMONXX_LIBS@ @LIBS@
LDFLAGS=@COMMONXX_LDFLAGS@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_PROGRAMS= Instanzen createDynEnums
EXTRA_PROGRAMS = checkGeliefert

Instanzen_SOURCES = Instanzen.cc
checkGeliefert_SOURCES= checkGeliefert_sql.cc
createDynEnums_SOURCES= createDynEnums.cc

CLEANFILES= *~ 

BUILT_SOURCES=checkGeliefert_sql.cc
