# $Id: Makefile.am,v 1.6 2002/10/24 14:08:59 thoma Exp $

CXXFLAGS=@CXXFLAGS@ @COMMONXX_INCLUDES@
LDFLAGS=@LDFLAGS@  @COMMONXX_LDFLAGS@
LDADD=@COMMONXX_LIBS@

EXTRA_PROGRAMS=graph

graph_SOURCES=graph.cc dot_out.cc get_data.cc

include_HEADERS=dot_out.h get_data.h

CLEANFILES= *~ $(EXTRA_PROGRAMS)

