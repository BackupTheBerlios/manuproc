# $Id: Makefile.am,v 1.8 2002/11/22 15:31:06 christof Exp $

CXXFLAGS=@CXXFLAGS@ @COMMONXX_INCLUDES@ 
LDADD=@COMMONXX_LIBS@ @LIBS@
LDFLAGS=@COMMONXX_LDFLAGS@

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
