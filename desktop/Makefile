PREFIX=/usr/local
DESKTOPDIR=$(PREFIX)/share/applnk/ManuProC
PIXMAPDIR=$(PREFIX)/share/pixmaps
MENUDIR=$(PREFIX)/lib/menu
BINDIR=$(PREFIX)/bin

all:

install:
	mkdir -p $(DESKTOPDIR) $(PIXMAPDIR) $(MENUDIR)
	for i in *.png ; do install -p $$i $(PIXMAPDIR) ; done
	for i in *.desktop ; do install -p $$i $(DESKTOPDIR) ; done
	@for i in *.menu ; \
		do \
		  name=`echo $$i | sed -e s/.menu//g` ; \
		  sed -e 's!@PIXMAPDIR@!$(PIXMAPDIR)!g' \
				-e 's!@BINDIR@!$(BINDIR)!g' \
			 	<$$i >$(MENUDIR)/$$name ; \
		  echo creating $(MENUDIR)/$$name ; \
		done
	@echo "please run as root: update-menus --menufiledir $(MENUDIR)/"
