SUBDIRS=artikelbox eingabe tree_simplest MVC newtree

EXTRA_PROGRAMS=Handles Event owntreemodel test_sts datewintest Message

Handles_SOURCES=Handles.cc
Event_SOURCES=Event.cc
owntreemodel_SOURCES=owntreemodel.cc
test_sts_SOURCES=test_sts.cc
datewintest_SOURCES=datewintest.cc
Message_SOURCES=Message.cc

AM_CXXFLAGS = @KOMPONENTEN2_INCLUDES@ 
Handles_LDADD = @KOMPONENTEN2_LIBS@
Event_LDADD = @KOMPONENTEN2_LIBS@
owntreemodel_LDADD = @GTKMM2_LIBS@ @COMMONXX_LIBS@
test_sts_LDADD = @KOMPONENTEN2_LIBS@
datewintest_LDADD = @KOMPONENTEN2_LIBS@
Message_LDADD = @KOMPONENTEN2_LIBS@
AM_LDFLAGS= @KOMPONENTEN2_LDFLAGS@

LINKS=Handles.cc Event.cc test_sts.cc datewintest.cc

BUILT_SOURCES=$(LINKS)

$(LINKS):
	ln -sf ../Basic/examples/$@ .

