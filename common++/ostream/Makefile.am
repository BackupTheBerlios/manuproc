
noinst_LIBRARIES=libostream.a

includedir=@includedir@/ostream

libostream_a_SOURCES=time_t_ostr.cc

libostream.a: $(libostream_a_OBJECTS) $(libostream_a_DEPENDENCIES)
	(for i in $(libostream_a_OBJECTS) ; do echo ostream/$$i ; done ) >$@
        
include_HEADERS=c++ostream.h

CLEANFILES= *~

BUILT_SOURCES=c++ostream.h time_t_ostr.cc

c++ostream.h time_t_ostr.cc:
	-ln -sf ../Basic/src/c++ostream.h ../Basic/src/time_t_ostr.cc .
 