# $Id: Makefile.am,v 1.14 2002/02/05 17:15:52 christof Exp $

CXXFLAGS=-s @ECPG_INCLUDES@ @CXXFLAGS@

LDADD=../libcommon++.a @ECPG_LIBS@ @ECPG_LDFLAGS@

tests: $(EXTRA_PROGRAMS)

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

EXTRA_PROGRAMS=preis test_Bankauftrag test_pgsql_conv testHandles \
	testExtBezHandle testKW fixedpoint artbez \
	Zeitpunkt insert_bestellen_bei FetchIStream abschreiben \
	ArtikelSumme long instanz_auftrag instanz_ArtikelBaum RL_lager \
   Jumbo_lager Lieferschein ppsInstanz trans

ppsInstanz_SOURCES=ppsInstanz.cc
Lieferschein_SOURCES=Lieferschein.cc
Jumbo_lager_SOURCES=Jumbo_lager.cc
RL_lager_SOURCES=RL_lager.cc
instanz_ArtikelBaum_SOURCES=instanz_ArtikelBaum.cc
instanz_auftrag_SOURCES=instanz_auftrag.cc
preis_SOURCES=preis.cc
artbez_SOURCES=artbez.cc
test_Bankauftrag_SOURCES=test_Bankauftrag.cc
test_pgsql_conv_SOURCES=test_pgsql_conv.cc
testHandles_SOURCES=testHandles.cc
testExtBezHandle_SOURCES=testExtBezHandle.cc
testKW_SOURCES=testKW.cc
insert_bestellen_bei_SOURCES=insert_bestellen_bei.cc
FetchIStream_SOURCES=FetchIStream.cc
fixedpoint_SOURCES=fixedpoint.cc
Zeitpunkt_SOURCES=Zeitpunkt.cc
abschreiben_SOURCES=abschreiben.cc
ArtikelSumme_SOURCES=ArtikelSumme.cc
long_SOURCES=long.cc
trans_SOURCES=trans.cc

CLEANFILES= *~ $(LINKS)

LINKS=testHandles.cc testKW.cc test_pgsql_conv.cc FetchIStream.pgcc \
	fixedpoint.cc Zeitpunkt.cc long.cc
#insert_bestellen_bei.pgcc

$(LINKS):
	ln -sf ../Basic/examples/$@ .

BUILT_SOURCES=FetchIStream.cc insert_bestellen_bei.cc
