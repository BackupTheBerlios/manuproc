# $Id: Makefile.am,v 1.24 2006/10/31 16:03:00 christof Exp $

AM_CXXFLAGS= @COMMONXX_INCLUDES@ 
LDADD=@LIBS@ @COMMONXX_LIBS@
AM_LDFLAGS= @COMMONXX_LDFLAGS@


%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_PROGRAMS= checkInstanzen Instanzen createDynEnums \
	db_upgrade auftrag_repair createDynDescr \
	warp_order_align createDynConfig blz_einlesen pgxml \
	manuproc_callback
EXTRA_PROGRAMS = checkGeliefert Zuordnung_zeigen adjust_store \
	auftrag_serialize

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
auftrag_serialize_SOURCES=auftrag_serialize.cc
manuproc_callback_SOURCES=manuproc_callback.cc

CLEANFILES= *~ 

BUILT_SOURCES=checkGeliefert_sql.cc
