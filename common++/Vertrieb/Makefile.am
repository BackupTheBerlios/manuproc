CXXFLAGS=@CXXFLAGS@ @ECPG_INCLUDES@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_LIBRARIES=libVertrieb.a

includedir=@includedir@/Vertrieb

libVertrieb_a_SOURCES=$(BUILT_SOURCES) ProvAbrechnung.cc

libVertrieb.a: $(libVertrieb_a_OBJECTS) $(libVertrieb_a_DEPENDENCIES) Makefile
	(for i in $(libVertrieb_a_OBJECTS) ; do echo Vertrieb/$$i ; done ) >$@
        
BUILT_SOURCES=ProvAbrechnung_sql.cc ProvAbrechnungEntry_sql.cc

include_HEADERS=ProvAbrechnung.h ProvAbrechnungEntry.h

CLEANFILES= *~ *.pct *.prot $(BUILT_SOURCES)

