CXXFLAGS=@CXXFLAGS@ @ECPG_INCLUDES@ @SIGC_CFLAGS@

includedir=@includedir@/BaseObjects

noinst_LIBRARIES=libBaseObjects.a

include_HEADERS=ManuProcEintrag.h ManuProcEntity.h ManuProcVorgang.h MVC.h

#libBaseObjects_a_SOURCES=ManuProcEntity.cc

libBaseObjects.a: $(libLieferschein_a_OBJECTS) $(libBaseObjects_a_DEPENDENCIE)
	touch $@

CLEANFILES= *~ 

BUILT_SOURCES=MVC.h

MVC.h: 
	-ln -sf ../Basic/src/MVC.h .
