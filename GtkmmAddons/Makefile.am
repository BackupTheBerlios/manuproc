# $Id: Makefile.am,v 1.4 2004/04/29 14:24:40 christof Exp $

SUBDIRS=src tests 

EXTRA_DIST = AUTHORS TODO README configure searchcombo/src/gtksearchcombo.c \
		searchcombo/src/gtksearchcombo.h

#INCLUDES = -I/home/share/safeSTL
#LDADD = -lefence

CLEANFILES = *~

mydist: clean
	(cd ..;mv gtk $(PACKAGE)-$(VERSION);\
	 tar --exclude \*CVS\* -cvzf $(PACKAGE)-$(VERSION).tgz $(PACKAGE)-$(VERSION);\
	 mv $(PACKAGE)-$(VERSION) gtk \
	)
