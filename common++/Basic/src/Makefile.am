# $Id: Makefile.am,v 1.63 2006/08/03 11:18:28 christof Exp $

if WINDOWS
WIN_LDFLAGS=-no-undefined -export-symbols libManuProC_Base@DATABASE_TYPE@.def 
DLL_LDADD=libManuProC_Base_res.lo @SIGC_LIBS@ @ECPG_LIBS@ 
DLL_DEF=libManuProC_Base@DATABASE_TYPE@.def
else
WIN_LDFLAGS=
DLL_LDADD=@SIGC_LIBS@ @ECPG_LIBS@
DLL_DEF=
endif

libManuProC_Base_sqlite3.def: libManuProC_Base.def libManuProC_Base_sqlite.defadd \
		libManuProC_Base_sigc@SIGC_MAJOR@.defadd
	cat >$@ $^
libManuProC_Base_res.lo: libManuProC_Base.rc
	WINDRES=i586-mingw32msvc-windres $(top_srcdir)/build/win32/lt-compile-resource $< $@

AM_CXXFLAGS=@ECPG_INCLUDES@ @SIGC_CFLAGS@ -DlibManuProC_Base_COMPILATION 

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

lib_LTLIBRARIES=libManuProC_Base.la
libManuProC_Base_la_LIBADD=$(DLL_LDADD)

includedir=@includedir@/ManuProC_Base

AM_LDFLAGS=@LDFLAGS@ @ECPG_LDFLAGS@ $(WIN_LDFLAGS)

if DATABASE
DB_SRCES=dbconnect.cc SQLerror_postgres.cc Transaction.cc Query.cc \
	Event.cc Global_Settings_db.cc dbconnect_shared.cc \
	FetchIStream_common.cc SQLerror_sqlite.cc ExtraColumns.cc
else 
DB_SRCES=
endif

libManuProC_Base_la_SOURCES=Ausgabe_neu.cc Datum.cc IOerror.cc \
		postgres_conversion.cc Zeitpunkt_new.cc \
		Zeitpunkt_new_ostr.cc \
		Zeitpunkt_new_write.cc string0.c mystring.cc \
		auto_conversion.cc \
		exception.cc itos.cc \
		time_t_ostr.cc EntryValueBase.cc Handles.cc \
		EntryValueIntString.cc EntryValueIntGermanString.cc \
		EntryValueDatum.cc $(DB_SRCES) Trace.cc \
		germanstring.cc TagStream.cc Tag.cc EntryValueTimeStamp.cc \
		Global_Settings.cc EntryValueSort.cc create_parse.cc \
		TS_base64.cc $(DLL_DEF) Interval.cc fixedpoint.cc \
		ManuProcEntity.cc

EXTRA_DIST=dbconnect.pgcc SQLerror_postgres.pgcc Transaction.pgcc \
	Query.pgcc

BUILT_SOURCES=Misc ostream $(DB_SRCES) $(DLL_DEF)

include_HEADERS=Ausgabe_neu.h CacheStatic.h Cache_Pointer.h Datum.h Handles.h \
	IOerror.h Kalenderwoche.h Pointer.h Postgres.h Rundungsmacros.h \
	SQLerror.h SQLerror_postgres.h Transaction.h Zeitpunkt_new.h \
	dbconnect.h exception.h fixedpoint.h itos.h \
	safemap.h string0.h mystring.h \
	c++ostream.h mystream.h FetchIStream.h ctime_assert.h \
	EntryValueBase.h EntryValueIntString.h EntryValueEmptyInt.h \
	EntryValueDatum.h EmptyInt_4TeX.h Long.h Model.h \
	germanstring.h TagStream.h Tag.h recodestream.h \
	callbackbuf.h TraceNV.h EntryValueTimeStamp.h \
	Event.h bitmask.h Global_Settings.h algorithm.h \
	ManuProcEintrag.h ManuProcEntity.h ManuProcVorgang.h

CLEANFILES= *~ $(BUILT_SOURCES)

# create compatibility links

Misc ostream BaseObjects:
	-if uname -s | fgrep -q MINGW32 ; \
	then mkdir $@ ; cp *.h $@ ; \
	else ln -sf . $@ ; \
	fi

install-data-hook:
	-cd $(includedir) ; ln -sf . Misc
	-cd $(includedir) ; ln -sf . ostream
	-cd $(includedir) ; ln -sf . BaseObjects

# the links cannot be distributed ... and they are not rebuild automatically
Ausgabe_neu.cc: Misc ostream BaseObjects
