# $Id: Makefile.am,v 1.19 2004/05/03 12:38:03 christof Exp $
	
LDADD=$(top_srcdir)/src/libManuProC_Base.la @ECPG_LIBS@ @SIGC_LIBS@

AM_CXXFLAGS=@CXXFLAGS@ -I$(top_srcdir)/src @ECPG_INCLUDES@ @SIGC_CFLAGS@

AM_LDFLAGS=@LDFLAGS@ @ECPG_LDFLAGS@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

if DATABASE
DB_PRGS=FetchIStream
B_SRCES=FetchIStream.cc
else
DB_PRGS=
B_SRCES=
endif

EXTRA_PROGRAMS=FetchIStream test_pgsql_conv testHandles testKW fixedpoint \
	Zeitpunkt mvc trace rexml

test_pgsql_conv_SOURCES=test_pgsql_conv.cc
testHandles_SOURCES=testHandles.cc
testKW_SOURCES=testKW.cc
fixedpoint_SOURCES=fixedpoint.cc
Zeitpunkt_SOURCES=Zeitpunkt.cc
trace_SOURCES=trace.cc
FetchIStream_SOURCES=$(B_SRCES)
mvc_SOURCES=mvc.cc
rexml_SOURCES=rexml.cc

CLEANFILES= *~ $(EXTRA_PROGRAMS)

BUILT_SOURCES=$(B_SRCES)

EXTRA_DIST=FetchIStream.pgcc
