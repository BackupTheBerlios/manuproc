CXXFLAGS=@CXXFLAGS@ @ECPG_INCLUDES@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_PROGRAMS=libExtBezSchema.o

includedir=@includedir@/ExtBezSchema

libExtBezSchema_o_SOURCES=$(BUILT_SOURCES)

libExtBezSchema.o: $(libExtBezSchema_o_OBJECTS) $(libExtBezSchema_o_DEPENDENCIES)
	touch $@

BUILT_SOURCES=ExtBezSchema_sql.cc

include_HEADERS=ExtBezSchema.h ExtBezSchemaHandle.h ArtikelTyp.h

CLEANFILES= *~ *.pct *.prot $(BUILT_SOURCES)

