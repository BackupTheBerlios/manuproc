SUBDIRS= BaseObjects DynamicEnums Aux ostream Artikel Auftrag Bank \
		ExtBezSchema Ketten \
		Kunde Lieferschein Material Lager Faeden Instanzen \
		. Programme tests 
	
lib_LIBRARIES=libcommon++.a

libcommon___a_SOURCES=

libcommon___a_LIBADD= \
		Artikel/ArtikelBaum_sql.o \
		Artikel/ArtikelBezeichnung.o \
      Artikel/ArtikelDispo_sql.o \
		Artikel/ArtikelMengeSumme.o \
		Artikel/ArtikelStamm.o \
		Artikel/ArtikelTyp.o \
		Artikel/Artikelpreis_sql.o \
		Artikel/Aufmachung.o \
		Artikel/Aufmachung_sql.o \
		Artikel/Einheiten.o \
		Artikel/Misc.o \
		Artikel/Preis.o \
		Artikel/Prozess_sql.o \
		Artikel/Prozesspreis.o \
		Artikel/ArtikelEAN_sql.o \
		Auftrag/AufEintragBase_sql.o \
		Auftrag/AufEintragBase.o \
		Auftrag/AufEintrag.o \
		Auftrag/AufEintrag_sql.o\
		Auftrag/selAufEntry_sql.o \
		Auftrag/sqlAuftragSelector.o \
		Auftrag/AuftragBase.o \
		Auftrag/AuftragBase_sql.o \
		Auftrag/AuftragsBaum.o \
		Auftrag/Auftrag.o \
		Auftrag/Auftrag_sql.o \
		Auftrag/AuftragFull_sql.o \
		Auftrag/AufEintragZu_sql.o \
      Aux/AdminProblems.o \
		Aux/Ausgabe_neu.o \
		Aux/Changejournal_sql.o \
		Aux/Datum.o \
		Aux/EntryValueBase.o \
		Aux/EntryValueDatum.o \
		Aux/EntryValueIntString.o \
		Aux/FetchIStream.o \
		Aux/Global_Settings.o \
		Aux/Handles.o \
		Aux/EAN.o \
		Aux/EAN_sql.o \
		Aux/IOerror.o \
		Aux/SQLerror_postgres.o \
		Aux/Transaction.o \
		Aux/Waehrung_sql.o \
		Aux/Zeitpunkt_new.o \
		Aux/Zeitpunkt_new_ostr.o \
		Aux/Zeitpunkt_new_write.o \
		Aux/auto_conversion.o \
		Aux/dbconnect.o \
		Aux/exception.o \
		Aux/itos.o \
		Aux/mystring.o \
		Aux/postgres_conversion.o \
		Aux/string0.o \
		Bank/Bankauftrag.o \
		Bank/Bankauftrag2.o \
		Bank/Datenfehler.o \
		Bank/Zahlvorgang.o \
		ExtBezSchema/ExtBezSchema_sql.o \
		Faeden/Bindung_sql.o \
		Faeden/Bindung.o \
		Faeden/Faden_sql.o \
		Faeden/Faden.o \
		Faeden/Materialeigenschaft_sql.o \
		Faeden/Schussfaeden_sql.o \
		Faeden/Webangaben.o \
		Faeden/Webangaben_sql.o \
		Instanzen/Produktionsdauer.o \
		Instanzen/ppsInstanzProduziert.o \
		Instanzen/ppsInstanz_sql.o \
      Ketten/ArtikelGang.o \
      Ketten/ArtikelGang_sql.o \
		Ketten/Kette.o \
		Ketten/KettenGarn.o \
		Ketten/Kettscheibe.o \
		Ketten/Kettscheibe_sql.o \
		Ketten/KettplanKette.o \
		Ketten/KettplanKette2.o \
		Ketten/KpK_get_artikel.o \
		Kunde/Adresse_sql.o \
		Kunde/Anrede_sql.o \
		Kunde/Kunde.o \
		Kunde/Kunde_sql.o \
		Kunde/LandesListe_sql.o \
		Kunde/Person.o \
		Kunde/Person_sql.o \
		Kunde/PreisListe_sql.o \
		Kunde/PreisListeFull_sql.o \
		Kunde/KontaktListe.o \
		Kunde/Telefon_sql.o \
		Kunde/Telefon.o \
		Kunde/Zahlungsart.o \
		Kunde/Zahlungsart_sql.o \
		Lager/FertigWaren_sql.o \
		Lager/FertigWarenLager_sql.o \
		Lager/JumboLager.o \
		Lager/JumboLager_sql.o \
		Lager/JumboRolle.o \
		Lager/JumboRolle_sql.o \
      Lager/LagerPlatz.o \
      Lager/Lager.o \
      Lager/Lager_Dispo_sql.o \
		Lager/Lager_Vormerkungen.o \
		Lager/maschinen_geschwindigkeit_sql.o \
      Lager/RohwarenLager_sql.o \
		Lieferschein/Lieferschein.o \
		Lieferschein/LieferscheinEntry.o \
		Lieferschein/LieferscheinEntry_sql.o \
		Lieferschein/LieferscheinList.o \
		Lieferschein/LieferscheinList_sql.o \
		Lieferschein/LieferscheinVoll.o \
		Lieferschein/Lieferschein_sql.o \
		Lieferschein/Rechnung.o \
		Lieferschein/Rechnung_sql.o \
		Lieferschein/RechnungEntry_sql.o \
		Lieferschein/RechnungVoll_sql.o \
		Lieferschein/Zahlungsart_sql.o \
		ostream/time_t_ostr.o

BUILT_SOURCES=Misc

Misc: Aux
	ln -sf Aux Misc
