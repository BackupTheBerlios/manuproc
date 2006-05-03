SUBDIRS=newtree tree_simplest dwhouse MVC

EXTRA_PROGRAMS = Event TreeViewUtil TreeSelection test_sts test_logwin \
	test_clist datewintest

AM_CXXFLAGS=@KOMPONENTEN_INCLUDES@ 
#-Wl,--enable-runtime-pseudo-reloc

Event_SOURCES=Event.cc
Event_LDADD=@KOMPONENTEN_LIBS@
TreeViewUtil_SOURCES=TreeViewUtil.cc
TreeViewUtil_LDADD=@KOMPONENTEN_LIBS@
TreeSelection_SOURCES=TreeSelection.cc
TreeSelection_LDADD=@KOMPONENTEN_LIBS@
test_sts_SOURCES=test_sts.cc
test_sts_LDADD=@KOMPONENTEN_LIBS@
test_logwin_SOURCES=test_logwin.cc
test_logwin_LDADD=@KOMPONENTEN_LIBS@
test_clist_SOURCES=test_clist.cc
test_clist_LDADD=@KOMPONENTEN_LIBS@
datewintest_SOURCES=datewintest.cc
datewintest_LDADD = @KOMPONENTEN_LIBS@

AM_LDFLAGS=@KOMPONENTEN_LDFLAGS@
