# $Id: Makefile.am,v 1.15 2002/12/11 16:53:51 christof Exp $
	
LDADD=$(top_srcdir)/src/libManuProC_Base.a @ECPG_LIBS@ @SIGC_LIBS@

CXXFLAGS=@CXXFLAGS@ -I$(top_srcdir)/src @ECPG_INCLUDES@ @SIGC_CFLAGS@

LDFLAGS=@LDFLAGS@ @ECPG_LDFLAGS@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

if DATABASE
DB_PRGS=FetchIStream
B_SRCES=FetchIStream.cc
else
DB_PRGS=
B_SRCES=
endif

EXTRA_PROGRAMS=FetchIStream test_pgsql_conv testHandles testKW fixedpoint \
	Zeitpunkt mvc trace

test_pgsql_conv_SOURCES=test_pgsql_conv.cc
testHandles_SOURCES=testHandles.cc
testKW_SOURCES=testKW.cc
fixedpoint_SOURCES=fixedpoint.cc
Zeitpunkt_SOURCES=Zeitpunkt.cc
trace_SOURCES=trace.cc
FetchIStream_SOURCES=$(B_SRCES)
mvc_SOURCES=mvc.cc

CLEANFILES= *~ 

BUILT_SOURCES=$(B_SRCES)

EXTRA_DIST=FetchIStream.pgcc
