SUBDIRS= DynamicEnums Misc BaseObjects ostream Artikel Auftrag Bank \
		ExtBezSchema Ketten \
		Kunde Lieferschein Material Lager Faeden Instanzen \
		Vertrieb . Programme tests 

	
lib_LIBRARIES=libcommon++.a

libcommon___a_SOURCES=

libcommon___a_DEPENDENCIES= Artikel/libArtikel.a Auftrag/libAuftrag.a \
	Bank/libBank.a BaseObjects/libBaseObjects.a \
	ExtBezSchema/libExtBezSchema.a Faeden/libFaeden.a \
	Instanzen/libInstanzen.a Auftrag/libAuftrag.a \
	Ketten/libKetten.a Kunde/libKunde.a Lager/libProdLager.a \
	Lieferschein/libLieferschein.a Misc/libMisc.a \
	ostream/libostream.a Vertrieb/libVertrieb.a

libcommon___a_LIBADD= \
		`cat $(libcommon___a_DEPENDENCIES)`

BUILT_SOURCES=Misc BaseObjects.link

BaseObjects.link:
	cd BaseObjects ; make Model.h ; cd .. ; touch BaseObjects.link

Misc:
	ln -sf Aux Misc
