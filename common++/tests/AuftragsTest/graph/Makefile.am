# $Id: Makefile.am,v 1.11 2002/11/22 15:47:37 thoma Exp $

CXXFLAGS=@CXXFLAGS@ @COMMONXX_INCLUDES@
LDFLAGS=@LDFLAGS@  @COMMONXX_LDFLAGS@
LDADD=@COMMONXX_LIBS@

EXTRA_PROGRAMS=graph

graph_SOURCES=graph.cc dot_out.cc get_data.cc

include_HEADERS=dot_out.h get_data.h

CLEANFILES= *~ $(EXTRA_PROGRAMS)

