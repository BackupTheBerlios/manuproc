# $Id: Makefile.am,v 1.7 2003/05/09 10:19:02 christof Exp $

CXXFLAGS=@CXXFLAGS@ @ECPG_INCLUDES@ 

%.cc: %.pgcc ; $(ECPG) -o $*.cc $<

noinst_LIBRARIES=libFaeden.a

includedir=@includedir@/Faeden

libFaeden_a_SOURCES= $(PGCC_Sources) \
	Faden.cc Bindung.cc Webangaben.cc

libFaeden.a : $(libFaeden_a_OBJECTS) $(libFaeden_a_DEPENDENCIES)
	(for i in $(libFaeden_a_OBJECTS) ; do echo Faeden/$$i ; done ) >$@

PGCC_Sources= Bindung_sql.cc Faden_sql.cc Schussfaeden_sql.cc \
   Materialeigenschaft_sql.cc Webangaben_sql.cc

BUILT_SOURCES= $(PGCC_Sources) 

include_HEADERS=Bindung.hh Faden.hh Schussfaeden.hh Materialeigenschaft.hh \
      Webangaben.hh


CLEANFILES=$(BUILT_SOURCES) *~
