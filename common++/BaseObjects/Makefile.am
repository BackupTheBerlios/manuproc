AM_CXXFLAGS=@ECPG_INCLUDES@ @SIGC_CFLAGS@

includedir=@includedir@/BaseObjects

noinst_LIBRARIES=libBaseObjects.a

include_HEADERS=ManuProcEintrag.h ManuProcEntity.h ManuProcVorgang.h MVC.h \
	ManuProcEntity_FetchIStream.h

libBaseObjects_a_SOURCES=ManuProcEntity.cc

libBaseObjects.a: $(libBaseObjects_a_OBJECTS) $(libBaseObjects_a_DEPENDENCIES) Makefile
	(for i in $(libBaseObjects_a_OBJECTS) ; do echo BaseObjects/$$i ; done ) >$@

CLEANFILES= *~ 

LINKS=MVC.h Model.h ModelPlex.h

BUILT_SOURCES=$(LINKS)

$(LINKS): 
	-ln -sf ../Basic/src/$@ .
