# $Id: Makefile.am,v 1.25 2004/09/24 22:52:15 christof Exp $
	
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
	Zeitpunkt mvc trace rexml tagstream tag_speed algorithm extra_cols

test_pgsql_conv_SOURCES=test_pgsql_conv.cc
testHandles_SOURCES=testHandles.cc
testKW_SOURCES=testKW.cc
fixedpoint_SOURCES=fixedpoint.cc
Zeitpunkt_SOURCES=Zeitpunkt.cc
trace_SOURCES=trace.cc
FetchIStream_SOURCES=$(B_SRCES)
mvc_SOURCES=mvc.cc
rexml_SOURCES=rexml.cc
tagstream_SOURCES=tagstream.cc
tag_speed_SOURCES=tag_speed.cc
algorithm_SOURCES=algorithm.cc
extra_cols_SOURCES=extra_cols.cc

tagstream.res: tagstream$(EXEEXT)
	./tagstream | sed -e 's/P0x[0-9a-fA-F]*/P*/' \
		-e 's/@0x[0-9a-fA-F]*/@*/' >tagstream.res
		
test_tagstream: tagstream.res
	diff -u tagstream.out tagstream.res | less

CLEANFILES= *~ $(EXTRA_PROGRAMS)

BUILT_SOURCES=$(B_SRCES)

EXTRA_DIST=FetchIStream.pgcc
