# $Id: Makefile.am,v 1.1 2001/04/23 08:11:59 christof Exp $

CXXFLAGS=@ECPG_INCLUDES@ @CXXFLAGS@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

lib_LIBRARIES=libManuProC_Base.a

includedir=@includedir@/ManuProC_Base

LDFLAGS=@LDFLAGS@ @ECPG_LDFLAGS@

libManuProC_Base_a_SOURCES=Ausgabe_neu.cc Datum.cc IOerror.cc \
		postgres_conversion.cc Zeitpunkt_new.cc \
		Zeitpunkt_new_ostr.cc \
		Zeitpunkt_new_write.cc string0.c \
		auto_conversion.cc $(PGCC_SOURCES) \
		exception.cc itos.cc fixedpoint.cc \
		time_t_ostr.cc

PGCC_SOURCES=   dbconnect.cc SQLerror_postgres.cc Transaction.cc \
	FetchIStream.cc
EXTRA_DIST=dbconnect.pgcc SQLerror_postgres.pgcc Transaction.pgcc \
	FetchIStream.pgcc

BUILT_SOURCES=Aux ostream $(PGCC_SOURCES)

include_HEADERS=Ausgabe_neu.h CacheStatic.h Cache_Pointer.h Datum.h Handles.h \
	IOerror.h Kalenderwoche.h Pointer.h Postgres.h Rundungsmacros.h \
	SQLerror.h SQLerror_postgres.h Transaction.h Zeitpunkt_new.h \
	dbconnect.h exception.h fixedpoint.h itos.h safemap.h string0.h \
	c++ostream.h mystream.h FetchIStream.h

CLEANFILES= *~ $(BUILT_SOURCES)

# create compatibility links

Aux ostream:
	-ln -s . $@

install-data-hook:
	-cd $(includedir) ; ln -sf . Aux
	-cd $(includedir) ; ln -sf . ostream

# the links cannot be distributed ... and they are not rebuild automatically
Ausgabe_neu.cc: Aux ostream
