# $Id: Makefile.am,v 1.9 2003/03/10 10:24:20 christof Exp $

AM_CXXFLAGS=@CXXFLAGS@ @COMMONXX_INCLUDES@ 
LDADD=@COMMONXX_LIBS@ @LIBS@
AM_LDFLAGS=@COMMONXX_LDFLAGS@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_PROGRAMS= checkInstanzen Instanzen createDynEnums adjust_store
EXTRA_PROGRAMS = checkGeliefert

checkInstanzen_SOURCES = checkInstanzen.cc
Instanzen_SOURCES = Instanzen.cc
checkGeliefert_SOURCES= checkGeliefert_sql.cc
createDynEnums_SOURCES= createDynEnums.cc
adjust_store_SOURCES= adjust_store.cc

CLEANFILES= *~ 

BUILT_SOURCES=checkGeliefert_sql.cc
