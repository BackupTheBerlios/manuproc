AM_CXXFLAGS=@ECPG_INCLUDES@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_LIBRARIES=libKetten.a

includedir=@includedir@/Ketten

libKetten_a_SOURCES=Kette.cc KettplanKette.cc Kettscheibe.cc ArtikelGang.cc \
		$(BUILT_SOURCES) KettenGarn.cc
	
libKetten.a: $(libKetten_a_OBJECTS) $(libKetten_a_DEPENDENCIES) Makefile
	(for i in $(libKetten_a_OBJECTS) ; do echo Ketten/$$i ; done ) >$@

BUILT_SOURCES=KettplanKette2.cc KpK_get_artikel.cc \
   ArtikelGang_sql.cc 

include_HEADERS=ArtikelGang.h Kette.h KettplanKette.h Maschtyp.h \
   Kettscheibe.h

CLEANFILES= *~ *.pct *.prot $(BUILT_SOURCES)

