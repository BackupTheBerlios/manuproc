/* $Id: AufEintrag.h,v 1.77 2003/12/03 15:01:25 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef CXX_AUFTRAG_AUFEINTRAG_H
#define CXX_AUFTRAG_AUFEINTRAG_H

// na ob das alles erforderlich ist ... CP
#include <Artikel/ArtikelBaum.h> //?
#include <Artikel/ArtikelBezeichnung.h> //?
#include <Artikel/Preis.h>
#include <Auftrag/AufEintragBase.h>
#include <Auftrag/auftrag_status.h>
#include <Misc/CacheStatic.h>
#include <Misc/Datum.h>
#include <Misc/Handles.h>
#include <Misc/Kalenderwoche.h>
#include <Misc/SQLerror.h>
#include <Misc/Waehrung.h>
#include <Kunde/Kunde.h>
#include <map>
#include <string>
#include <vector>
#include <list>
#include <Misc/itos.h>
#include <Auftrag/AufEintragZu.h>
#include <Auftrag/auftrag_enums.h>
#include <Misc/compiler_ports.h>
#include <Lieferschein/LieferscheinEntryBase.h> // wegen opt. arg. von unbestellteMengeProduzieren
#include <Auftrag/AufEintrag_loops.h>

class Lager;
class VerfuegbareMenge;
class ProductionContext;
class LieferscheinEntryBase;
class cH_Lieferschein;
typedef LieferscheinEntryBase ProductionContext2;

class AufEintrag : public AufEintragBase
{
     friend class AuftragBase;
     friend class ppsInstanzReparatur;
     friend class VerfuegbareMenge; // besser hier hin ...
     friend FetchIStream &operator>>(FetchIStream &is, AufEintrag &ae);

     class MichEinlagern;
     class AbbestellenUndVormerken;
     class Abbestellen_cb;
     class NeuBestellen;
     struct Auslagern_cb;
     class Einlagern_cb;
     class delayed_reclaim;
     class WiederEinlagern_cb;
     class MichProduzieren;
     class ProduziertRueckgaengig2;
     class ProduziertNG_cb2;
     class ArtikelAbbestellen_bevorzugt;
     class EinlagernRueckgaengig;
     class ArtikelInternAbbestellen_cb;

     #if ( __GNUC__ < 3 )
     friend class MichEinlagern;
     friend class AbbestellenUndVormerken;
     friend class Abbestellen_cb;
     friend class NeuBestellen;
     friend class Einlagern_cb;
     friend class delayed_reclaim;
     friend class WiederEinlagern_cb;
     friend class MichProduzieren;
     friend class ProduziertRueckgaengig2;
     friend class ProduziertNG_cb2;
     friend class ArtikelAbbestellen_bevorzugt;
     friend class EinlagernRueckgaengig;
     friend class ArtikelInternAbbestellen_cb;
     #endif


private: 
 mengen_t bestellt;
 mengen_t geliefert;
 ArtikelBase artikel;

 AufStatVal entrystatus; /* Status des zugehörigen Eintrags */
 ManuProC::Datum lieferdatum;
 

 int lasteditdate_uid;
 ManuProC::Datum lasteditdate, letzte_lieferung;
 ppsInstanz::ID letztePlanInstanz;
 int maxPlanInstanz;
 ManuProC::Datum prozdate;

 Preis preis;
 rabatt_t rabatt;
 mutable fixedpoint<2> provsatz;
 
 static bool lager_bevorzugt_freigeben;
 
private: // weg von hier!
 Kunde::ID kdnr;
 std::string youraufnr;
 int disponr;
 AufStatVal auftragstatus; /* Status des zugehörigen Auftrages */


 int dispoentrynr; // unnötig
 cH_Prozess prozess; // unnötig


 cH_PreisListe preisliste;
public:
 struct Error : public std::exception
   {  virtual const char* what() const throw() { return "AufEintrag::AufEintragError"; }
       Error() {}
   };
 struct NoAEB_Error : public std::exception
   {  int x; 
      virtual const char* what() const throw() 
        { return "AufEintrag::NoAEB_Error"; }
       NoAEB_Error(int i):x(i) {}
   };

 AufEintrag() 
   : bestellt(), geliefert(), artikel(), entrystatus((AufStatVal)UNCOMMITED),
   	letztePlanInstanz(ppsInstanzID::None),maxPlanInstanz(), rabatt(),
   	provsatz(-1),kdnr(), disponr(), auftragstatus((AufStatVal)UNCOMMITED),
   	dispoentrynr(),prozess(Prozess::default_id)
 {}
 
 // Dieser ctor ist für AuftragFull::push_back
 AufEintrag(const AufEintragBase &aeb, mengen_t _bestellt,
	ArtikelBase _artikel, const ManuProC::Datum _lieferdatum,
	AufStatVal _aufstatus, int _kdnr, const std::string _youraufnr,
	const Preis &_preis, rabatt_t _rabatt, AufStatVal _entrystat, 
	const cH_PreisListe &preisliste) throw();
 // Dieser Konstuktor ist nur für ProdLager gedacht und macht KEINEN Datenbankzugriff
 AufEintrag(ppsInstanz::ID _instanz,int _auftragid, int _zeilennr, 
        mengen_t _bestellt,
	ArtikelBase _artikel, const ManuProC::Datum _lieferdatum,
	AufStatVal _entrystat) throw();
 AufEintrag(const AufEintragBase &aebb) throw (SQLerror,NoAEB_Error);

	
 void updateDispoENr(int dinr) throw(SQLerror);
 mengen_t MengeAendern(mengen_t menge,bool instanzen,const AufEintragBase &ElternAEB) throw(SQLerror);
private:
  // nimmt alle Zuordnungen mit (oben & unten)
  void move_to(AufEintrag ziel,mengen_t menge) throw(std::exception);
 void updateStkDiffInstanz__(mengen_t menge) throw(SQLerror);
 void ArtikelInternAbbestellen(mengen_t menge) const;
 void move_menge_to_dispo_zuordnung_or_lager(mengen_t menge,const ArtikelBase artikel);
 // wurde von ProduziertNG abgelöst
 __deprecated void WurdeProduziert(mengen_t menge,const AufEintragBase &ElternAEB);

 class ArtikelInternAbbestellen_cb : public distribute_children_cb
 {	const AufEintrag &mythis;
	
  public:
	ArtikelInternAbbestellen_cb(const AufEintrag &_mythis)
		: mythis(_mythis)
	{}
	// das 1. Argument wird nicht verwendet
	mengen_t operator()(const ArtikelBase &,
 		const AufEintragBase &,AuftragBase::mengen_t) const;
 	// ignore remainder
 	void operator()(const ArtikelBase &,AuftragBase::mengen_t) const {}
 };

public:
 void updateLieferdatum(const ManuProC::Datum &ld) throw(SQLerror);	
 void updateLieferdatum(const Kalenderwoche &K) {updateLieferdatum(ManuProC::Datum(K));}	
 // was ist das denn? CP
 void updateLieferdatumInstanz(const ManuProC::Datum &ld) throw(SQLerror);	
 void updatePreis(const Preis &pr) throw(SQLerror);
 void updateRabatt(rabatt_t rb) throw(SQLerror);
 void setLetzteLieferung(const ManuProC::Datum &datum) throw(SQLerror);
 // Ist (uid!=0) wird lasteditdate verändert.
 void setStatus(AufStatVal newstatus,bool force=false) throw(SQLerror);		
// void setInstanzen(const AufStatVal newstatus,const Petig::Datum &lieferdate,const mengen_t &Menge,const int myznr=-1,const int yourznr=-1);

 int split(mengen_t newmenge, const ManuProC::Datum &newld,bool dispoplanung=false) throw(SQLerror);
 mengen_t getStueck() const { return bestellt;}
 mengen_t getRestStk() const; // Statusabhängig ...
 mengen_t getGeliefert() const { return geliefert;}
 AufStatVal getAufStatus() const { return auftragstatus; }
 int getZnr() const { return zeilennr;}
 int getAuftragid() const {return auftragid;}
 ppsInstanz::ID getAuftragInstanz() const {return instanz->Id();}
 // vermutlich ist getCombinedStatus() eher das gewollte ...
 AufStatVal getEntryStatus() const { return entrystatus;}
 // Resultierender Status (mit Auftrag)
 AufStatVal getCombinedStatus() const { return entrystatus!=OPEN?entrystatus:auftragstatus;}
 const std::string getEntryStatusStr() const;
 int getLastEditDateUID() const { return lasteditdate_uid;}
 const ManuProC::Datum LastEditDate() const { return lasteditdate; }
 const ManuProC::Datum LetzteLieferung() const { return letzte_lieferung; }
 std::string getYourAufNr() const { return youraufnr;}
 int getDispoENr() const { return dispoentrynr;}
 const ManuProC::Datum getLieferdatum() const { return lieferdatum;}
 Kunde::ID getKdNr() const { return kdnr;}
 const ManuProC::Datum getProzDat() const { return prozdate;} 
 cH_Prozess getProzess() const { return prozess;}
 const cP_Waehrung getWaehrung() const { return preis.getWaehrung(); }
 const Preis GPreis() const; // Gesamtpreis
// void setVerarbeitung(const cH_Prozess p);
 const Preis EPreis(bool brutto=true) const; // Einzelpreis
 cH_PreisListe getPreisliste() const {return preisliste;}
 rabatt_t Rabatt() const { return rabatt;}
 Preis::preismenge_t PreisMenge() const { return preis.PreisMenge(); }
 fixedpoint<2> ProvSatz() const throw(SQLerror);
 fixedpoint<2> DefaultProvSatz() const throw(SQLerror); 
 void setDefaultProvSatz() throw(SQLerror);
 void setProvSatz(const fixedpoint<2> ps) throw(SQLerror);

   friend class AufEintragBase;

// intern aber public
 void abschreiben(mengen_t menge) throw(SQLerror);
 
 // gibt überschüssige Menge zurück (ZI2)
 mengen_t AnElternMengeAnpassen();

private:
 void Produziert_0er(mengen_t menge);
 static void WiederEinlagern(cH_ppsInstanz instanz,const ArtikelBase artikel,
         mengen_t menge) throw(SQLerror);
 // bitte ProduziertNG aufrufen!         
 void Produziert(mengen_t menge,ManuProcEntity<>::ID lfrsid);
 void KinderProduzieren(mengen_t M, const AufEintragBase &neuerAEB,
			const ProductionContext2 &ctx);

public:
 // wird z.B. von push_back verwendet
 void ArtikelInternNachbestellen(mengen_t menge) const;

 bool allesOK() const;
 std::string Planung() const;
 ppsInstanz::ID LetztePlanInstanz() const { return letztePlanInstanz; }
 void setLetztePlanungFuer(cH_ppsInstanz planinstanz) throw(SQLerror);


 // wird von Artikeleingabe verwendet; gibt Zeilennummer zurück;
//Alter Code: void moveInstanz(const AuftragBase& auftrag) throw(SQLerror);

 // der Rückgabewert von cH_Lieferschein gefällt mir nicht, LieferscheinBase
 // ist IMHO besser, CP
 __deprecated cH_Lieferschein getLieferschein() const ;
private:
 std::vector<AufEintragBase> getKundenAuftragV() const;

 void LagerMenge_beruecksichtigen(const mengen_t &lagermengediff,const AufEintragBase &ElternAEB) ;

// muss beim abschreiben geschehen
 void DispoBeschraenken();

public:
 AufEintragBase getFirstKundenAuftrag() const;

 __deprecated ArtikelBase::ID ArtId() const {return artikel.Id();}
 ArtikelBase Artikel() const {return artikel;}

 static AufEintragBase default_opfer(cH_ppsInstanz i,mengen_t menge,const ArtikelBase &aeb);
 static AufEintragBase (*opfer_auswaehlen)(cH_ppsInstanz,mengen_t,const ArtikelBase &);

// einen Teil des Auftrages=0 verplanen (in anderen Auftrag<>0 setzen)
// gibt neue Zeile zurück; rekursiv = alle Instanzen darunter auch planen,
// rekursiv wird asuschließlich vom Erfassungs/Reperaturprogramm verwendet
 AufEintragBase Planen(mengen_t menge,const AuftragBase &zielauftrag,
      const ManuProC::Datum &datum) throw(std::exception);
 __deprecated AufEintragBase Planen(mengen_t menge,const AuftragBase &zielauftrag,
      const ManuProC::Datum &datum, 
      // diese Argumente können weg ... oder?
      ManuProC::Auftrag::Action reason,
      AufEintragBase *verplanter_aeb=0,bool rekursiv=false) throw(std::exception);
private:         
 __deprecated void ProduktionsPlanung(mengen_t menge,const AuftragBase &zielauftrag,
      const ManuProC::Datum &datum,cH_ppsInstanz instanz) throw(std::exception);
      
 // 2er anlegen, Material bestellen
 // *this ist der ZielAufEintrag
 void Ueberplanen(const ArtikelBase& artikel,mengen_t menge,const ManuProC::Datum &datum);

// Eine bereits vorgemerkte Menge einem anderen AufEintag zuordnen
// *this => Der reservierte 1er; ae => Der ungeplante 0er
//?? void menge_fuer_aeb_freigeben(const mengen_t &menge,AufEintrag &ae);

public:
 // brauche ich noch ein statisches ProduziertNG (das nur eine Instanz erhält?)
 // z.B. (getRestStk())
 void ProduziertNG(mengen_t M,const ProductionContext2 &ctx);
//internal ?
 void ProduziertNG(mengen_t M,
		const AufEintragBase &elter_alt,
		const AufEintragBase &elter_neu,
		const ProductionContext2 &ctx);
private:		
 void Einlagern2(mengen_t M,
		const AufEintragBase &elter_alt,
		const AufEintragBase &elter_neu,
		// hier drin stecken eventuelle Kinder
		const ProductionContext &ctx);
 static AufEintragBase unbestellteMengeProduzieren2(cH_ppsInstanz instanz,
 		const ArtikelBase &artikel,mengen_t menge,bool rekursiv,
 		const AufEintragBase &elter,const ProductionContext2 &ctx,
 		ManuProC::Datum termin);
 // neuen (geschlossenen) 1er erzeugen (völlig unverzeigert ?)
public:
 static AufEintragBase unbestellteMengeProduzieren(cH_ppsInstanz instanz,
 		const ArtikelBase &artikel,
 		mengen_t menge,bool rekursiv=false,
 		const AufEintragBase &elter=AufEintragBase(),
 		const ProductionContext2 &ctx=ProductionContext2(),
 		ManuProC::Datum termin=ManuProC::Datum());
 // aus dem Lager auslagern (abschreiben), ehemals abschreiben_oder_reduzieren
 // wird üblicherweise erst für 1er dann 2er aufgerufen
 // mit ProduziertNG vereinen?
 static mengen_t Auslagern
	(const AuftragBase &ab,const ArtikelBase &artikel,mengen_t menge, 
		bool fuer_auftraege,
		const ProductionContext2 &ctx);
 mengen_t Auslagern(mengen_t menge, const ProductionContext &ctx,
 							bool fuer_auftraege=true);
   // wird aufgerufen wenn Menge ins Lager kommt (LagerBase::rein_ins_lager)
   // kümmert sich um 1er und 2er
   // sollte Aufträge als produziert markieren
   // ehemals AuftragBase::menge_neu_verplanen
   static void Einlagern(cH_ppsInstanz instanz,const ArtikelBase artikel,
         const mengen_t &menge,bool produziert,
         const ProductionContext &ctx) throw(SQLerror);
   // Menge wurde als Produziert markiert, kam aber ins Lager zurück
private:
// intern aber public wegen der klassen
   static AufEintragBase ArtikelInternNachbestellen(const cH_ppsInstanz &wo,
 	mengen_t menge,const ManuProC::Datum &lieferdatum,const ArtikelBase& artikel,
 	const AufEintragBase& ElternAEB);
   static void MengeVormerken(cH_ppsInstanz instanz,const ArtikelBase &artikel,
		mengen_t menge, bool abbestellen, const ProductionContext &ctx);
   // für ProduziertSelbst Instanzen
   mengen_t ProdRueckgaengigMenge(mengen_t) const;
   // ehemals increase_parents__reduce_assingments
   void MengeNeubestellen(mengen_t menge) throw(SQLerror);
};

#endif
