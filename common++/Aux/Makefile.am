# $Id: Makefile.am,v 1.6 2001/06/22 09:52:02 christof Exp $

CXXFLAGS=@ECPG_INCLUDES@ @CXXFLAGS@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_PROGRAMS=libAux.o

includedir=@includedir@/Aux

libAux_o_SOURCES= Ausgabe_neu.cc Datum.cc IOerror.cc \
		postgres_conversion.cc Zeitpunkt_new.cc \
		Zeitpunkt_new_ostr.cc EntryValue.cc \
		Zeitpunkt_new_write.cc string0.c \
		EntryValueBase.cc EntryValueIntString.cc \
		auto_conversion.cc $(PGCC_SOURCES) \
		exception.cc itos.cc

libAux.o: $(libAux_o_OBJECTS) $(libAux_o_DEPENDENCIES)
	touch $@

PGCC_SOURCES=   dbconnect.cc SQLerror_postgres.cc Transaction.cc \
		Waehrung_sql.cc Changejournal_sql.cc FetchIStream.cc

LINKS= Ausgabe_neu.cc Ausgabe_neu.h CacheStatic.h Cache_Pointer.h \
	Datum.cc Datum.h Handles.h IOerror.cc IOerror.h Kalenderwoche.h Pointer.h \
	Postgres.h Rundungsmacros.h SQLerror.h SQLerror_postgres.h \
	SQLerror_postgres.pgcc Transaction.h Transaction.pgcc Zeitpunkt_new.cc \
	Zeitpunkt_new.h Zeitpunkt_new_ostr.cc Zeitpunkt_new_write.cc \
	auto_conversion.cc dbconnect.h dbconnect.pgcc \
	exception.cc exception.h fixedpoint.h mystream.h \
	postgres_conversion.cc safemap.h string0.c string0.h itos.cc itos.h \
	FetchIStream.pgcc FetchIStream.h ctime_assert.h \
   EntryValueBase.cc EntryValueBase.h EntryValueIntString.cc \
   EntryValueIntString.h EntryValueEmptyInt.h

$(LINKS):
	ln -sf ../Basic/src/$@ .

BUILT_SOURCES=$(PGCC_SOURCES) $(LINKS)

include_HEADERS=Ausgabe.h Ausgabe_neu.h Datum.h \
		IOerror.h Postgres.h \
		Rundungsmacros.h SQLerror.h \
		SQLerror_postgres.h Zeitpunkt_new.h \
		EntryValue.h dbconnect.h Kalenderwoche.h mystream.h \
		Handles.h Cache_Pointer.h CacheStatic.h \
		EntryValueBase.h safemap.h string0.h \
		Transaction.h Waehrung.h Pointer.h \
		Changejournal.h ppsInstanz.h EntryValueIntString.h \
		exception.h itos.h fixedpoint.h FetchIStream.h \
		ctime_assert.h EntryValueEmptyInt.h

CLEANFILES= *~ $(BUILT_SOURCES)

