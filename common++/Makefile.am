SUBDIRS= DynamicEnums Misc BaseObjects ostream Artikel Auftrag Bank \
		ExtBezSchema Ketten \
		Kunde Lieferschein Material Lager Faeden Instanzen \
		Vertrieb . Programme tests 
	
lib_LIBRARIES=libcommon++.a

libcommon___a_SOURCES=

libcommon___a_LIBADD= \
		`cat Artikel/libArtikel.a` \
		`cat Auftrag/libAuftrag.a` \
		`cat Bank/libBank.a` \
		`cat BaseObjects/libBaseObjects.a` \
		`cat ExtBezSchema/libExtBezSchema.a` \
		`cat Faeden/libFaeden.a` \
		`cat Instanzen/libInstanzen.a` \
		`cat Auftrag/libAuftrag.a` \
		`cat Ketten/libKetten.a` \
		`cat Kunde/libKunde.a` \
		`cat Lager/libLager.a` \
		`cat Lieferschein/libLieferschein.a` \
		`cat Misc/libMisc.a` \
		`cat ostream/libostream.a` \
		`cat Vertrieb/libVertrieb.a` 

BUILT_SOURCES=Misc

Misc:
	ln -sf Aux Misc
