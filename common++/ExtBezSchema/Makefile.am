AM_CXXFLAGS=@ECPG_INCLUDES@

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

noinst_LIBRARIES=libExtBezSchema.a

includedir=@includedir@/ExtBezSchema

libExtBezSchema_a_SOURCES=$(BUILT_SOURCES)

libExtBezSchema.a: $(libExtBezSchema_a_OBJECTS) $(libExtBezSchema_a_DEPENDENCIES) Makefile
	(for i in $(libExtBezSchema_a_OBJECTS) ; do echo ExtBezSchema/$$i ; done ) >$@

BUILT_SOURCES=ExtBezSchema_sql.cc

include_HEADERS=ExtBezSchema.h ExtBezSchemaHandle.h ArtikelTyp.h

CLEANFILES= *~ *.pct *.prot $(BUILT_SOURCES)

