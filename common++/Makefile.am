SUBDIRS= BaseObjects DynamicEnums Misc ostream Artikel Auftrag Bank \
		ExtBezSchema Ketten \
		Kunde Lieferschein Material Lager Faeden Instanzen \
		Vertrieb . Programme tests 
	
lib_LIBRARIES=libcommon++.a

libcommon___a_SOURCES=

libcommon___a_LIBADD= \
		Artikel/ArtikelBase.o \
		Artikel/ArtikelBaum.o \
		Artikel/ArtikelBaum_new_del.o \
		Artikel/ArtikelBezeichnung.o \
      Artikel/ArtikelDispo_sql.o \
		Artikel/ArtikelEAN_sql.o \
		Artikel/ArtikelMengeSumme.o \
		Artikel/Artikelpreis_sql.o \
		Artikel/ArtikelStamm.o \
		Artikel/ArtikelTyp.o \
		Artikel/Aufmachung.o \
		Artikel/Aufmachung_sql.o \
		Artikel/Einheiten.o \
		Artikel/Misc.o \
		Artikel/Preis.o \
		Artikel/Prozesspreis.o \
		Artikel/Prozess_sql.o \
      Auftrag/VerfuegbareMenge.o \
		Auftrag/AufEintragBase.o \
		Auftrag/AufEintrag.o \
		Auftrag/AufEintrag_ro.o\
		Auftrag/AufEintragZuMengenAenderung.o \
		Auftrag/AufEintragZu.o \
		Auftrag/AuftragBase.o \
		Auftrag/AuftragBase_sql.o \
		Auftrag/AuftragFull_sql.o \
		Auftrag/Auftrag.o \
		Auftrag/AuftragsBaum.o \
		Auftrag/Auftrag_sql.o \
		Auftrag/selAufEntry_sql.o \
		Auftrag/sqlAuftragSelector.o \
		Bank/Bankauftrag2.o \
		Bank/Bankauftrag.o \
		Bank/Datenfehler.o \
		Bank/Zahlvorgang.o \
		ExtBezSchema/ExtBezSchema_sql.o \
		Faeden/Bindung.o \
		Faeden/Bindung_sql.o \
		Faeden/Faden.o \
		Faeden/Faden_sql.o \
		Faeden/Materialeigenschaft_sql.o \
		Faeden/Schussfaeden_sql.o \
		Faeden/Webangaben.o \
		Faeden/Webangaben_sql.o \
		Instanzen/ppsInstanz.o \
      Auftrag/ppsInstanzReparatur.o \
		Instanzen/ppsInstanz_sql.o \
		Instanzen/Produktionsdauer.o \
      Ketten/ArtikelGang.o \
      Ketten/ArtikelGang_sql.o \
		Ketten/KettenGarn.o \
		Ketten/Kette.o \
		Ketten/KettplanKette2.o \
		Ketten/KettplanKette.o \
		Ketten/Kettscheibe.o \
		Ketten/Kettscheibe_sql.o \
		Ketten/KpK_get_artikel.o \
		Kunde/Adresse_sql.o \
		Kunde/Anrede_sql.o \
		Kunde/KontaktListe.o \
		Kunde/Kundengruppe.o \
		Kunde/Kunde.o \
		Kunde/Kunde_sql.o \
		Kunde/LandesListe_sql.o \
		Kunde/Person.o \
		Kunde/Person_sql.o \
		Kunde/PreisListeFull_sql.o \
		Kunde/PreisListe_sql.o \
		Kunde/Telefon.o \
		Kunde/Telefon_sql.o \
		Kunde/Zahlungsart.o \
		Kunde/Zahlungsart_sql.o \
		Lager/FertigWarenLager_sql.o \
		Lager/FertigWaren.o \
		Lager/FertigWaren_sql.o \
		Lager/JumboLager.o \
		Lager/JumboLager_sql.o \
		Lager/JumboRolle.o \
		Lager/JumboRolle_sql.o \
      Lager/Lager_Dispo_sql.o \
      Lager/Lager.o \
      Lager/LagerPlatz.o \
		Lager/maschinen_geschwindigkeit_sql.o \
      Lager/RohwarenLager.o \
      Lager/RohwarenLager_sql.o \
		Lieferschein/LieferscheinEntry.o \
		Lieferschein/LieferscheinEntry_sql.o \
		Lieferschein/LieferscheinList.o \
		Lieferschein/LieferscheinList_sql.o \
		Lieferschein/Lieferschein.o \
		Lieferschein/Lieferschein_sql.o \
		Lieferschein/LieferscheinVoll.o \
		Lieferschein/RechnungEntry_sql.o \
		Lieferschein/Rechnung.o \
		Lieferschein/Rechnung_sql.o \
		Lieferschein/RechnungVoll_sql.o \
		Lieferschein/Zahlungsart_sql.o \
      Misc/AdminProblems.o \
		Misc/Ausgabe_neu.o \
		Misc/auto_conversion.o \
		Misc/Changejournal_sql.o \
		Misc/Datum.o \
		Misc/dbcapability.o \
		Misc/dbconnect.o \
		Misc/EAN.o \
		Misc/EAN_sql.o \
		Misc/EntryValueBase.o \
		Misc/EntryValueDatum.o \
		Misc/EntryValueIntString.o \
		Misc/EntryValueQuartal.o \
		Misc/exception.o \
		Misc/FetchIStream.o \
		Misc/FetchIStream_ops.o \
		Misc/Global_Settings.o \
		Misc/Global_Settings_sql.o \
		Misc/Handles.o \
		Misc/IOerror.o \
		Misc/itos.o \
		Misc/multi_lang.o \
		Misc/mystring.o \
		Misc/postgres_conversion.o \
		Misc/SQLerror_postgres.o \
		Misc/string0.o \
      Misc/Trace.o \
		Misc/Transaction.o \
		Misc/Waehrung_sql.o \
		Misc/Zeitpunkt_new.o \
		Misc/Zeitpunkt_new_ostr.o \
		Misc/Zeitpunkt_new_write.o \
		ostream/time_t_ostr.o \
	Vertrieb/ProvAbrechnungEntry_sql.o \
	Vertrieb/ProvAbrechnung.o \
	Vertrieb/ProvAbrechnung_sql.o 

BUILT_SOURCES=Misc

Misc:
	ln -sf Aux Misc
