CXXFLAGS=@CXXFLAGS@ @ECPG_INCLUDES@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_LIBRARIES=libKetten.a

includedir=@includedir@/Ketten

libKetten_a_SOURCES=Kette.cc KettplanKette.cc \
		$(BUILT_SOURCES)
	
libKetten.a: $(libKetten_a_OBJECTS) $(libKetten_a_DEPENDENCIES)
	touch $@

BUILT_SOURCES=KettplanKette2.cc KpK_get_artikel.cc

include_HEADERS=ArtikelGang.h Kette.h KettplanKette.h Maschtyp.h

CLEANFILES= *~ *.pct *.prot $(BUILT_SOURCES)

