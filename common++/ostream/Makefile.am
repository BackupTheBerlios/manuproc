
noinst_PROGRAMS=libostream.o

includedir=@includedir@/ostream

libostream_o_SOURCES=time_t_ostr.cc

libostream.o: $(libostream_o_OBJECTS) $(libostream_o_DEPENDENCIES)
	touch $@
        
include_HEADERS=c++ostream.h

CLEANFILES= *~

BUILT_SOURCES=c++ostream.h time_t_ostr.cc

c++ostream.h time_t_ostr.cc:
	-ln -sf ../Basic/src/c++ostream.h ../Basic/src/time_t_ostr.cc .
 