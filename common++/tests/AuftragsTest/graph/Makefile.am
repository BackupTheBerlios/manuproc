# $Id: Makefile.am,v 1.12 2003/07/30 09:24:54 christof Exp $

AM_CXXFLAGS=@COMMONXX_INCLUDES@
AM_LDFLAGS=@COMMONXX_LDFLAGS@
LDADD=@COMMONXX_LIBS@

EXTRA_PROGRAMS=graph

graph_SOURCES=graph.cc dot_out.cc get_data.cc

include_HEADERS=dot_out.h get_data.h

CLEANFILES= *~ $(EXTRA_PROGRAMS)

