/* $Id: AufEintrag.h,v 1.20 2002/11/25 15:21:52 thoma Exp $ */
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

#ifndef CXX_AUFTRAG_AUFEINTRAGBASE_H
#define CXX_AUFTRAG_AUFEINTRAGBASE_H

// na ob das alles erforderlich ist ... CP
#include <Artikel/ArtikelBaum.h> //?
#include <Artikel/ArtikelBezeichnung.h> //?
#include <Artikel/Preis.h>
#include <Auftrag/AufEintragBase.h>
#include <Auftrag/auftrag_status.h>
#include <Aux/CacheStatic.h>
#include <Aux/Datum.h>
#include <Aux/Handles.h>
#include <Aux/Kalenderwoche.h>
#include <Aux/SQLerror.h>
#include <Aux/Waehrung.h>
#include <Kunde/Kunde.h>
#include <map>
#include <string>
#include <vector>
#include <list>
#include <Aux/itos.h>
class cH_Lieferschein;
#include <Auftrag/AufEintragZu.h>
#include <Auftrag/auftrag_enums.h>


class Lager;

class AufEintrag : public AufEintragBase
{
//     friend class LagerBase;
     friend class AuftragBase;
     friend FetchIStream &operator>>(FetchIStream &is, AufEintrag &ae);

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

private: // weg von hier!
 int kdnr;
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
   : bestellt(0), geliefert(0), artikel(0ul), entrystatus((AufStatVal)UNCOMMITED),
   	letztePlanInstanz(ppsInstanzID::None),maxPlanInstanz(0), rabatt(0),
   	kdnr(0), disponr(0), auftragstatus((AufStatVal)UNCOMMITED), 
   	dispoentrynr(0),prozess(Prozess::default_id)
 {}
 
 // Dieser ctor ist für AuftragFull::push_back
 AufEintrag(const AufEintragBase &aeb, mengen_t _bestellt,
	ArtikelBase _artikel, const ManuProC::Datum _lieferdatum,
	AufStatVal _aufstatus, int _kdnr, const std::string _youraufnr,
	const Preis &_preis, rabatt_t _rabatt, AufStatVal _entrystat, 
	int uid, const cH_PreisListe &preisliste) throw();
 // Dieser Konstuktor ist nur für ProdLager gedacht und macht KEINEN Datenbankzugriff
 AufEintrag(ppsInstanz::ID _instanz,int _auftragid, int _zeilennr, 
        mengen_t _bestellt,
	ArtikelBase _artikel, const ManuProC::Datum _lieferdatum,
	AufStatVal _entrystat) throw();
 AufEintrag(const AufEintragBase &aebb) throw (SQLerror,NoAEB_Error);

	
 void updateDispoENr(int dinr) throw(SQLerror);
 mengen_t updateStkDiff__(int uid,mengen_t menge,bool instanzen,ManuProC::Auftrag::Action reason=ManuProC::Auftrag::r_None) throw(SQLerror);
private:
// void move_to(int uid,AufEintragBase AEB,AuftragBase::mengen_t menge,bool reduce_old) throw(std::exception);
 void move_to(int uid,AufEintragBase AEB,AuftragBase::mengen_t menge,ManuProC::Auftrag::Action reason) throw(std::exception);
 void updateStkDiffInstanz__(int uid,mengen_t menge,ManuProC::Auftrag::Action reason) throw(SQLerror);
 void move_menge_to_dispo_zuordnung_or_lager(mengen_t menge,int uid,ManuProC::Auftrag::Action reason);

public:
 void updateLieferdatum(const ManuProC::Datum &ld,int uid) throw(SQLerror);	
 void updateLieferdatum(const Kalenderwoche &K,int uid) {updateLieferdatum(ManuProC::Datum(K),uid);}	
 void updateLieferdatumInstanz(const ManuProC::Datum &ld) throw(SQLerror);	
 void updatePreis(const Preis &pr) throw(SQLerror);
 void updateRabatt(rabatt_t rb) throw(SQLerror);
 void setLetzteLieferung(const ManuProC::Datum &datum) throw(SQLerror);
 // Ist (uid!=0) wird lasteditdate verändert.
 void setStatus(AufStatVal newstatus,int uid,bool force=false) throw(SQLerror);		
// void setInstanzen(AufStatVal newstatus,int uid,ManuProC::Datum lieferdate,mengen_t part,int myznr=-1,int yourznr=-1);
 void setInstanzen(const AufStatVal newstatus,const int uid,const Petig::Datum &lieferdate,const mengen_t &Menge,const int myznr=-1,const int yourznr=-1);

 int split(int uid,mengen_t newmenge, const ManuProC::Datum &newld,bool dispoplanung=false) throw(SQLerror);
 mengen_t getStueck() const { return bestellt;}
 mengen_t getRestStk() const {if(entrystatus==CLOSED)return 0; return bestellt-geliefert;}
 mengen_t getGeliefert() const { return geliefert;}
 AufStatVal getAufStatus() const { return auftragstatus; }
 int getZnr() const { return zeilennr;}
 int getAuftragid() const {return auftragid;}
 ppsInstanz::ID getAuftragInstanz() const {return instanz->Id();}
 AufStatVal getEntryStatus() const { return entrystatus;}
 const std::string getEntryStatusStr() const;
 int getLastEditDateUID() const { return lasteditdate_uid;}
 const ManuProC::Datum LastEditDate() const { return lasteditdate; }
 const ManuProC::Datum LetzteLieferung() const { return letzte_lieferung; }
 std::string getYourAufNr() const { return youraufnr;}
 int getDispoENr() const { return dispoentrynr;}
 const ManuProC::Datum getLieferdatum() const { return lieferdatum;}
 int getKdNr() const { return kdnr;}
 const ManuProC::Datum getProzDat() const { return prozdate;} 
 cH_Prozess getProzess() const { return prozess;}
 const cP_Waehrung getWaehrung() const { return preis.getWaehrung(); }
 const Preis GPreis() const; // Gesamtpreis
// void setVerarbeitung(const cH_Prozess p);
 const Preis EPreis(bool brutto=true) const; // Einzelpreis
 cH_PreisListe getPreisliste() const {return preisliste;}
 rabatt_t Rabatt() const { return rabatt;}
 Preis::preismenge_t PreisMenge() const { return preis.PreisMenge(); }

private: 
   friend struct ManuProC::st_produziert;
   friend void ppsInstanz::Produziert(ManuProC::st_produziert &P,ManuProC::Auftrag::Action reason) const;
   friend class AufEintragBase;
 void abschreiben(mengen_t menge,
    ManuProcEntity<>::ID lfrsid) throw(SQLerror);

public:
 void Produziert(mengen_t menge,
     ManuProcEntity<>::ID lfrsid)  throw(SQLerror);


public:
 bool allesOK() const;
 std::string Planung() const;
 ppsInstanz::ID LetztePlanInstanz() const { return letztePlanInstanz; }
 void setLetztePlanungFuer(cH_ppsInstanz planinstanz) throw(SQLerror);


 // wird von Artikeleingabe verwendet; gibt Zeilennummer zurück;
 void moveInstanz(int uid,const AuftragBase& auftrag) throw(SQLerror);

 cH_Lieferschein getLieferschein() const ;
private:
 std::vector<AufEintragBase> getKundenAuftragV() const;
public:
 AufEintragBase getFirstKundenAuftrag() const;

 ArtikelBase::ID ArtId() const {return artikel.Id();}
 ArtikelBase Artikel() const {return artikel;}

// einen Teil des Auftrages=0 verplanen (in anderen Auftrag<>0 setzen)
// gibt Zeilennummer zurück; rekursiv = alle Instanzen darunter auch planen,
// rekursiv wird asuschließlich vom Erfassungs/Reperaturprogramm verwendet
// Wenn 'reduce_old=true' wird von *this die Menge reduziert
 int Planen(int uid,mengen_t menge,const AuftragBase &zielauftrag,
      const ManuProC::Datum &datum, ManuProC::Auftrag::Action reason=ManuProC::Auftrag::r_Planen,
         AufEintragBase *verplanter_aeb=0,bool rekursiv=false) throw(std::exception);
 void ProduktionsPlanung(int uid,mengen_t menge,const AuftragBase &zielauftrag,
      const ManuProC::Datum &datum,cH_ppsInstanz instanz) throw(std::exception);

// Eine bereits vorgemerkte Menge einem anderen AufEintag zuordnen
// *this => Der reservierte 1er; ae => Der ungeplante 0er
 void menge_fuer_aeb_freigeben(const mengen_t &menge,AufEintrag &ae,const int uid);

};

#endif
