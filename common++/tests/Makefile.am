# $Id: Makefile.am,v 1.1 2001/04/23 08:11:59 christof Exp $

CXXFLAGS=@ECPG_INCLUDES@ @CXXFLAGS@

LDADD=../libcommon++.a @ECPG_LIBS@ @ECPG_LDFLAGS@


%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_PROGRAMS=preis test_Bankauftrag test_pgsql_conv testHandles \
	testExtBezHandle testKW FetchIStream

preis_SOURCES=preis.cc
test_Bankauftrag_SOURCES=test_Bankauftrag.cc
test_pgsql_conv_SOURCES=test_pgsql_conv.cc
testHandles_SOURCES=testHandles.cc
testExtBezHandle_SOURCES=testExtBezHandle.cc
testKW_SOURCES=testKW.cc
FetchIStream_SOURCES=FetchIStream.cc

CLEANFILES= *~ $(LINKS)

LINKS=testHandles.cc testKW.cc test_pgsql_conv.cc FetchIStream.pgcc 	

$(LINKS):
	ln -sf ../Basic/examples/$@ .

BUILT_SOURCES=FetchIStream.cc

