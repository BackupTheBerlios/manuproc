# $Id: Makefile.am,v 1.5 2002/06/20 06:29:53 christof Exp $

CXXFLAGS=@CXXFLAGS@ @ECPG_INCLUDES@ 

%.cc: %.pgcc ; $(ECPG) -o $*.cc $<

noinst_LIBRARIES=libFaeden.a

includedir=@includedir@/Faeden

libFaeden_a_SOURCES= $(PGCC_Sources) \
	Faden.cc Bindung.cc

libFaeden.a : $(libFaeden_a_OBJECTS) $(libFaeden_a_DEPENDENCIES)
	touch $@

PGCC_Sources= Bindung_sql.cc Faden_sql.cc Schussfaeden.cc \
   Materialeigenschaft_sql.cc

BUILT_SOURCES= $(PGCC_Sources) 

include_HEADERS=Bindung.hh Faden.hh Schussfaeden.hh Materialeigenschaft.hh


CLEANFILES=$(BUILT_SOURCES) *~
