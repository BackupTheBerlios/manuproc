# generated 2004/3/12 22:15:24 CET by jacek@jaceksdell.(none)
# using glademm V2.0.0b
#
# newer (non customized) versions of this file go to Makefile.am_new
bin_PROGRAMS = mpc_agent


if WINDOWS
#PORT_FILES=mpc_agent.rc
#LINK_OPTIONS=-mwindows -Wl,--enable-runtime-pseudo-reloc
LINK_OPTIONS= -Wl,--enable-runtime-pseudo-reloc
#LINK_OBJS=mpc_agent.o
#WIN_INCLUDES=
#-I/usr/local/mingw/mingw32/include -I/usr/local/mingw/local/include/gdk
#mpc_agent.o: i586-mingw32msvc-windres mpc_agent.cc

else
PORT_FILES=
LINK_OPTIONS=
LINK_OBJS=   
WIN_INCLUDES=
endif


mpc_agent_SOURCES = \
	mpc_agent_glade.cc \
	mpc_agent.cc \
	mpc_internal.cc

noinst_HEADERS = \
	mpc_agent_glade.hh \
	mpc_agent.hh 

AM_CXXFLAGS = @KOMPONENTEN2_INCLUDES@ 
AM_CPPFLAGS = @KOMPONENTEN2_INCLUDES@ $(WIN_INCLUDES) -I..
AM_LDFLAGS = @KOMPONENTEN2_LDFLAGS@ $(LINK_OPTIONS) -lsqlite

mpc_agent_LDADD = $(LINK_OBJS) @LIBS@ @KOMPONENTEN2_LIBS@

