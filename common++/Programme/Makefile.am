# $Id: Makefile.am,v 1.22 2005/11/04 09:56:04 christof Exp $

AM_CXXFLAGS= @COMMONXX_INCLUDES@ 
LDADD=@LIBS@ @COMMONXX_LIBS@
AM_LDFLAGS= @COMMONXX_LDFLAGS@


%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_PROGRAMS= checkInstanzen Instanzen createDynEnums \
	db_upgrade auftrag_repair createDynDescr \
	warp_order_align createDynConfig blz_einlesen pgxml
EXTRA_PROGRAMS = checkGeliefert Zuordnung_zeigen adjust_store

checkInstanzen_SOURCES = checkInstanzen.cc
Instanzen_SOURCES = Instanzen.cc
checkGeliefert_SOURCES= checkGeliefert_sql.cc
createDynEnums_SOURCES= createDynEnums.cc
createDynDescr_SOURCES= createDynDescr.cc
createDynConfig_SOURCES=createDynConfig.cc
adjust_store_SOURCES= auftrag_repair.cc
auftrag_repair_SOURCES= auftrag_repair.cc
Zuordnung_zeigen_SOURCES=Zuordnung_zeigen.cc
db_upgrade_SOURCES=db_upgrade.cc
warp_order_align_SOURCES=warp_order_align.cc
blz_einlesen_SOURCES=blz_einlesen.cc
pgxml_SOURCES=pgxml.cc

CLEANFILES= *~ 

BUILT_SOURCES=checkGeliefert_sql.cc
