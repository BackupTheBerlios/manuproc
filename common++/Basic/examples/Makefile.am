# $Id: Makefile.am,v 1.1 2001/04/23 08:11:59 christof Exp $
	
LDADD=$(top_srcdir)/src/libManuProC_Base.a @ECPG_LIBS@

CXXFLAGS=@CXXFLAGS@ -I$(top_srcdir)/src @ECPG_INCLUDES@

LDFLAGS=@LDFLAGS@ @ECPG_LDFLAGS@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_PROGRAMS=test_pgsql_conv testHandles testKW FetchIStream

test_pgsql_conv_SOURCES=test_pgsql_conv.cc
testHandles_SOURCES=testHandles.cc
testKW_SOURCES=testKW.cc
FetchIStream_SOURCES=FetchIStream.cc

CLEANFILES= *~ 

BUILT_SOURCES=FetchIStream.cc

EXTRA_DIST=FetchIStream.pgcc
