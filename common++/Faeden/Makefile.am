# $Id: Makefile.am,v 1.1 2001/10/01 13:03:01 christof Exp $

CXXFLAGS=@CXXFLAGS@ @ECPG_INCLUDES@ 

%.cc: %.pgcc ; $(ECPG) -o $*.cc $<

noinst_PROGRAMS=libFaeden.o

includedir=@includedir@/Faeden

libFaeden_o_SOURCES= $(PGCC_SOURCES) $(BUILT_SOURCES)

libFaeden.o : $(libFaeden_o_OBJECTS) $(libFaeden_o_DEPENDENCIES)
	touch $@

PGCC_SOURCES= Bindung_sql.cc Faden_sql.cc Schussfaeden.cc \
   Materialeigenschaft_sql.cc

BUILT_SOURCES= $(PGCC_SOURCES) Faden.cc Bindung.cc

include_HEADERS=Bindung.hh Faden.hh Schussfaeden.hh Materialeigenschaft.hh


CLEANFILES=$(BUILT_SOURCES) *~
