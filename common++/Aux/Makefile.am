# $Id: Makefile.am,v 1.47 2003/11/05 09:48:45 christof Exp $

AM_CXXFLAGS=@ECPG_INCLUDES@ @CXXFLAGS@ @SIGC_CFLAGS@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_LIBRARIES=libMisc.a

includedir=@includedir@/Misc

PGCC_Sources=dbconnect.cc SQLerror_postgres.cc Transaction.cc \
		Waehrung_sql.cc Changejournal_sql.cc FetchIStream.cc \
      Global_Settings_sql.cc EAN_sql.cc 

libMisc_a_SOURCES= Ausgabe_neu.cc Datum.cc IOerror.cc \
		postgres_conversion.cc Zeitpunkt_new.cc \
		Zeitpunkt_new_ostr.cc Global_Settings.cc \
		Zeitpunkt_new_write.cc string0.c mystring.cc \
		EntryValueBase.cc EntryValueIntString.cc \
		auto_conversion.cc $(PGCC_Sources) AdminProblems.cc \
		exception.cc itos.cc EntryValueDatum.cc Handles.cc EAN.cc \
		Trace.cc FetchIStream_ops.cc \
		dbcapability.cc Tag.cc TagStream.cc \
		multi_lang.cc EntryValueTimeStamp.cc \
		Event.cc germanstring.cc EntryValueIntGermanString.cc

libMisc.a: $(libMisc_a_OBJECTS) $(libMisc_a_DEPENDENCIES) Makefile
	(for i in $(libMisc_a_OBJECTS) ; do echo Misc/$$i ; done ) >$@

LINKS= Ausgabe_neu.cc Ausgabe_neu.h CacheStatic.h Cache_Pointer.h \
	Datum.cc Datum.h Handles.h Handles.cc \
	IOerror.cc IOerror.h Kalenderwoche.h Pointer.h \
	Postgres.h Rundungsmacros.h SQLerror.h SQLerror_postgres.h \
	SQLerror_postgres.pgcc Transaction.h Transaction.pgcc Zeitpunkt_new.cc \
	Zeitpunkt_new.h Zeitpunkt_new_ostr.cc Zeitpunkt_new_write.cc \
	auto_conversion.cc dbconnect.h dbconnect.pgcc \
	exception.cc exception.h fixedpoint.h mystream.h \
	postgres_conversion.cc safemap.h string0.c string0.h mystring.cc mystring.h itos.cc itos.h \
	FetchIStream.pgcc FetchIStream.h ctime_assert.h \
   EntryValueBase.cc EntryValueBase.h EntryValueIntString.cc \
   EntryValueIntString.h EntryValueEmptyInt.h EntryValueDatum.h \
   EntryValueDatum.cc EmptyInt_4TeX.h EntryValueFixed.h Long.h \
   Trace.h Trace.cc relops.h EntryValueFixedEinheit.h UniqueValue.h \
   compiler_ports.h FILEstream.h TagStream.cc TagStream.h Tag.cc Tag.h \
   TraceNV.h TimeStamp.h EntryValueTimeStamp.cc EntryValueTimeStamp.h \
   Event.h Event.cc inbetween.h germanstring.h germanstring.cc \
   EntryValueIntGermanString.cc EntryValueIntGermanString.h \
   recodestream.h bitmask.h TemporaryValue.h vectormap.h intmap.h \
   libManuProC_Base_dll.h Global_Settings.h Global_Settings.cc \
   Global_Settings_sql.pgcc

$(LINKS):
	ln -sf ../Basic/src/$@ .

BUILT_SOURCES=$(LINKS) $(PGCC_Sources) 

include_HEADERS=Ausgabe.h Ausgabe_neu.h Datum.h \
		IOerror.h Postgres.h \
		Rundungsmacros.h SQLerror.h \
		SQLerror_postgres.h Zeitpunkt_new.h \
		dbconnect.h Kalenderwoche.h mystream.h \
		Handles.h Cache_Pointer.h CacheStatic.h \
		EntryValueBase.h safemap.h string0.h mystring.h \
		Transaction.h Waehrung.h Pointer.h \
		Changejournal.h EntryValueIntString.h \
		exception.h itos.h fixedpoint.h FetchIStream.h \
		ctime_assert.h EntryValueEmptyInt.h EntryValueDatum.h \
		EntryValueFixed.h Global_Settings.h EAN.h AdminProblems.h \
		EntryValueFixedEinheit.h \
		dbcapability.h \
		multi_lang.h EntryValueTimeStamp.h inbetween.h \
		bitmask.h TemporaryValue.h vectormap.h germanstring.cc \
		EntryValueIntGermanString.cc EntryValueIntGermanString.h \
		recodestream.h intmap.h libManuProC_Base_dll.h

CLEANFILES= *~ $(BUILT_SOURCES)

