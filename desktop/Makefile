PREFIX=/usr/local
DESKTOPDIR=$(PREFIX)/share/applnk/ManuProC
PIXMAPDIR=$(PREFIX)/share/pixmaps

all:

install:
	for i in *.png ; do install -p $$i $(PIXMAPDIR) ; done
	for i in *.desktop ; do install -p $$i $(DESKTOPDIR) ; done
