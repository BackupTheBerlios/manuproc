# $Id: Makefile.am,v 1.10 2002/11/22 15:22:00 thoma Exp $

CXXFLAGS=@CXXFLAGS@ @COMMONXX_INCLUDES@
LDFLAGS=@LDFLAGS@  @COMMONXX_LDFLAGS@
LDADD=@COMMONXX_LIBS@

EXTRA_PROGRAMS=graph

graph_SOURCES=graph.cc dot_out.cc get_data.cc

include_HEADERS=dot_out.h get_data.h

CLEANFILES= *~ $(EXTRA_PROGRAMS)

