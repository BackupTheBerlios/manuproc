# $Id: Makefile.am,v 1.6 2002/05/09 12:46:01 christof Exp $

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
