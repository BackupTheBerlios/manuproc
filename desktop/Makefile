PREFIX=/usr/local
DESKTOPDIR=$(PREFIX)/share/applnk/ManuProC
PIXMAPDIR=$(PREFIX)/share/pixmaps
MENUDIR=$(PREFIX)/lib/menu

all:

install:
	mkdir -p $(DESKTOPDIR) $(PIXMAPDIR) $(MENUDIR)
	for i in *.png ; do install -p $$i $(PIXMAPDIR) ; done
	for i in *.desktop ; do install -p $$i $(DESKTOPDIR) ; done
	for i in *.menu ; \
		do \
		  name=`echo $$i | sed -e s/.menu//g` ; \
		  sed -e 's!@PIXMAPDIR@!$(PIXMAPDIR)!g' <$$i >$(MENUDIR)/$$name ; \
		done
