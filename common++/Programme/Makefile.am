# $Id: Makefile.am,v 1.11 2003/05/23 11:43:53 christof Exp $

AM_CXXFLAGS=@CXXFLAGS@ @COMMONXX_INCLUDES@ 
LDADD=@LIBS@ @COMMONXX_LIBS@
LDFLAGS=@LDFLAGS@ @COMMONXX_LDFLAGS@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_PROGRAMS= checkInstanzen Instanzen createDynEnums adjust_store
EXTRA_PROGRAMS = checkGeliefert Zuordnung_zeigen

checkInstanzen_SOURCES = checkInstanzen.cc
Instanzen_SOURCES = Instanzen.cc
checkGeliefert_SOURCES= checkGeliefert_sql.cc
createDynEnums_SOURCES= createDynEnums.cc
adjust_store_SOURCES= adjust_store.cc
Zuordnung_zeigen_SOURCES=Zuordnung_zeigen.cc

CLEANFILES= *~ 

BUILT_SOURCES=checkGeliefert_sql.cc
