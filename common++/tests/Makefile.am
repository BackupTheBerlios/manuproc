# $Id: Makefile.am,v 1.3 2001/06/06 07:27:39 christof Exp $

CXXFLAGS=-s @ECPG_INCLUDES@ @CXXFLAGS@

LDADD=../libcommon++.a @ECPG_LIBS@ @ECPG_LDFLAGS@


%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_PROGRAMS=preis test_Bankauftrag test_pgsql_conv testHandles \
	testExtBezHandle testKW FetchIStream fixedpoint

preis_SOURCES=preis.cc
test_Bankauftrag_SOURCES=test_Bankauftrag.cc
test_pgsql_conv_SOURCES=test_pgsql_conv.cc
testHandles_SOURCES=testHandles.cc
testExtBezHandle_SOURCES=testExtBezHandle.cc
testKW_SOURCES=testKW.cc
FetchIStream_SOURCES=FetchIStream.cc
fixedpoint_SOURCES=fixedpoint.cc

CLEANFILES= *~ $(LINKS)

LINKS=testHandles.cc testKW.cc test_pgsql_conv.cc FetchIStream.pgcc \
	fixedpoint.cc

$(LINKS):
	ln -sf ../Basic/examples/$@ .

BUILT_SOURCES=FetchIStream.cc

