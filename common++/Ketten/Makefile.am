CXXFLAGS=@CXXFLAGS@ @ECPG_INCLUDES@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_PROGRAMS=libKetten.o

includedir=@includedir@/Ketten

libKetten_o_SOURCES=Kette.cc KettplanKette.cc \
		$(BUILT_SOURCES)
	
libKetten.o: $(libKetten_o_OBJECTS) $(libKetten_o_DEPENDENCIES)
	touch $@

BUILT_SOURCES=KettplanKette2.cc KpK_get_artikel.cc

include_HEADERS=ArtikelGang.h Kette.h KettplanKette.h Maschtyp.h

CLEANFILES= *~ *.pct *.prot $(BUILT_SOURCES)

