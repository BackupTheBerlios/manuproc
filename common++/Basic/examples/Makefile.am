# $Id: Makefile.am,v 1.11 2002/07/08 08:34:09 christof Exp $
	
LDADD=$(top_srcdir)/src/libManuProC_Base.a @ECPG_LIBS@

CXXFLAGS=@CXXFLAGS@ -I$(top_srcdir)/src @ECPG_INCLUDES@

LDFLAGS=@LDFLAGS@ @ECPG_LDFLAGS@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

if DATABASE
DB_PRGS=FetchIStream
B_SRCES=FetchIStream.cc
else
DB_PRGS=
B_SRCES=
endif

EXTRA_PROGRAMS=FetchIStream

noinst_PROGRAMS=test_pgsql_conv testHandles testKW fixedpoint \
	Zeitpunkt mvc

test_pgsql_conv_SOURCES=test_pgsql_conv.cc
testHandles_SOURCES=testHandles.cc
testKW_SOURCES=testKW.cc
fixedpoint_SOURCES=fixedpoint.cc
Zeitpunkt_SOURCES=Zeitpunkt.cc
FetchIStream_SOURCES=$(B_SRCES)
mvc_SOURCES=mvc.cc

CLEANFILES= *~ 

BUILT_SOURCES=$(B_SRCES)

EXTRA_DIST=FetchIStream.pgcc
