# $Id: Makefile.am,v 1.15 2003/07/07 07:56:42 christof Exp $

AM_CXXFLAGS=@CXXFLAGS@ @COMMONXX_INCLUDES@ 
LDADD=@LIBS@ @COMMONXX_LIBS@
LDFLAGS=@LDFLAGS@ @COMMONXX_LDFLAGS@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_PROGRAMS= checkInstanzen Instanzen createDynEnums \
	db_upgrade auftrag_repair
EXTRA_PROGRAMS = checkGeliefert Zuordnung_zeigen adjust_store

checkInstanzen_SOURCES = checkInstanzen.cc
Instanzen_SOURCES = Instanzen.cc
checkGeliefert_SOURCES= checkGeliefert_sql.cc
createDynEnums_SOURCES= createDynEnums.cc
adjust_store_SOURCES= auftrag_repair.cc
auftrag_repair_SOURCES= auftrag_repair.cc
Zuordnung_zeigen_SOURCES=Zuordnung_zeigen.cc
db_upgrade_SOURCES=db_upgrade.cc

CLEANFILES= *~ 

BUILT_SOURCES=checkGeliefert_sql.cc
