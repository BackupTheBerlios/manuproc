# $Id: Makefile.am,v 1.10 2003/04/14 13:33:47 jacek Exp $

AM_CXXFLAGS=@CXXFLAGS@ @COMMONXX_INCLUDES@ 
LDADD=@LIBS@ @COMMONXX_LIBS@
LDFLAGS=@LDFLAGS@ @COMMONXX_LDFLAGS@

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
