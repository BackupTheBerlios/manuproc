# $Id: Makefile.am,v 1.7 2002/06/20 06:29:53 christof Exp $

CXXFLAGS=@CXXFLAGS@ @COMMONXX_INCLUDES@ 
LDADD=@COMMONXX_LIBS@ @LIBS@
LDFLAGS=@COMMONXX_LDFLAGS@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_PROGRAMS= Instanzen createDynEnums adjust_store
EXTRA_PROGRAMS = checkGeliefert

Instanzen_SOURCES = Instanzen.cc
checkGeliefert_SOURCES= checkGeliefert_sql.cc
createDynEnums_SOURCES= createDynEnums.cc
adjust_store_SOURCES= adjust_store.cc

CLEANFILES= *~ 

BUILT_SOURCES=checkGeliefert_sql.cc
