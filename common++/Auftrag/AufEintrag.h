/* $Id: AufEintrag.h,v 1.6 2002/05/06 13:41:22 christof Exp $ */
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
#include <Lieferschein/Lieferschein.h>

class AufEintrag : public AufEintragBase
{
 public:
  enum e_problems{Geplant,Geplant_nolager,Lager,Geliefert,GeliefertFatal};
  struct st_problems{e_problems art; AufEintragBase AEB; mengen_t menge_input;
                     mengen_t menge_output;
         st_problems(e_problems a, AufEintragBase aeb, mengen_t mi,mengen_t mo)
               :art(a),AEB(aeb),menge_input(mi),menge_output(mo) {}};
private: 
 mengen_t bestellt;
 mengen_t geliefert;
 ArtikelBase artikel;

 AufStatVal entrystatus; /* Status des zugehörigen Eintrags */
 
 Petig::Datum lieferdatum;

 Petig::Datum lasteditdate;
 ppsInstanz::ID letztePlanInstanz;
 int maxPlanInstanz;
 Petig::Datum prozdate;

 Preis preis;
 rabatt_t rabatt;     // % * 100

private: // weg von hier!
 int kdnr;
 std::string youraufnr;
 int disponr;
 AufStatVal auftragstatus; /* Status des zugehörigen Auftrages */

 // unnötig 	
 int dispoentrynr; // unnötig
 cH_Prozess prozess; // unnötig

public:
 struct Error : public std::exception
   {  virtual const char* what() const throw() { return "AufEintrag::AufEintragError"; }
       Error() {}
   };
 struct NoAEB_Error : public std::exception
   {  int x; 
      virtual const char* what() const throw() 
        { return string("AufEintrag::NoAEB_Error: "+itos(x)).c_str(); }
       NoAEB_Error(int i):x(i) {}
   };

 AufEintrag() 
   : bestellt(0), geliefert(0), artikel(0ul), entrystatus((AufStatVal)UNCOMMITED),
   	letztePlanInstanz(ppsInstanzID::None),maxPlanInstanz(0), rabatt(0),
   	kdnr(0), disponr(0), auftragstatus((AufStatVal)UNCOMMITED), 
   	dispoentrynr(0),prozess(Prozess::default_id)
 {}
 
 AufEintrag(ppsInstanz::ID _instanz,int _auftragid, int _zeilennr, 
        mengen_t _bestellt,
	ArtikelBase _artikel, const Petig::Datum _lieferdatum,
	mengen_t _geliefert,
	int _dispoentrynr, int _disponr, 
	AufStatVal _aufstatus,
	int _kdnr, const std::string _youraufnr,
	const Petig::Datum& _prozdate,
	int _prozess,ppsInstanz::ID _letztePlanInstanz, int _maxPlanInstanz,
	const Preis &_preis, int _rabatt,
	AufStatVal _entrystat, const Petig::Datum _lasteditdate) throw();
 // Dieser Konstuktor ist nur für ProdLager gedacht und macht KEINEN Datenbankzugriff
 AufEintrag(ppsInstanz::ID _instanz,int _auftragid, int _zeilennr, 
        mengen_t _bestellt,
	ArtikelBase _artikel, const Petig::Datum _lieferdatum,
	AufStatVal _entrystat) throw();
 AufEintrag(const AufEintragBase &aebb) throw (SQLerror,NoAEB_Error);

	
 void updateDispoENr(int dinr) throw(SQLerror);
 mengen_t updateStkDiff__(mengen_t menge,bool instanzen=true,
           void (*callback)(void *,st_problems)=0,void* argument=0) throw(SQLerror);
 void move_to(AufEintragBase AEB,AuftragBase::mengen_t menge) throw(SQLerror);
// void updateStk(mengen_t stk,bool instanz) throw(SQLerror);
private:
// void updateStkInstanz(mengen_t neu_stk,mengen_t alt_stk) throw(SQLerror);
 void updateStkDiffInstanz__(mengen_t menge,void (*callback)(void *,st_problems),void* argument) throw(SQLerror);
public:
 void updateLieferdatum(const Petig::Datum &ld) throw(SQLerror);	
 void updateLieferdatum(const Kalenderwoche &K) {updateLieferdatum(Petig::Datum(K));}	
 void updateLieferdatumInstanz(const Petig::Datum &ld) throw(SQLerror);	
 void updatePreis(const Preis &pr) throw(SQLerror);
 void updateRabatt(int rb) throw(SQLerror);
 void setStatus(AufStatVal newstatus,bool force=false) throw(SQLerror);		
 void setInstanzen(AufStatVal newstatus,Petig::Datum lieferdate,mengen_t part,int myznr=-1,int yourznr=-1);
 void split(mengen_t newmenge, const Petig::Datum &newld,void (*callback)(void *,st_problems)=0,void* argument=0) throw(SQLerror);
 mengen_t getStueck() const { return bestellt;}
 mengen_t getRestStk() const {if(entrystatus==CLOSED)return 0; return bestellt-geliefert;}
 mengen_t getGeliefert() const { return geliefert;}
 AufStatVal getAufStatus() const { return auftragstatus; }
 int getZnr() const { return zeilennr;}
 int getAuftragid() const {return auftragid;}
 ppsInstanz::ID getAuftragInstanz() const {return instanz->Id();}
 AufStatVal getEntryStatus() const { return entrystatus;}
 const std::string getEntryStatusStr() const;
 const Petig::Datum LastEditDate() const { return lasteditdate; }
 std::string getYourAufNr() const { return youraufnr;}
 int getDispoENr() const { return dispoentrynr;}
 const Petig::Datum getLieferdatum() const { return lieferdatum;}
 int getKdNr() const { return kdnr;}
 const Petig::Datum getProzDat() const { return prozdate;} 
 cH_Prozess getProzess() const { return prozess;}
 const cP_Waehrung getWaehrung() const { return preis.getWaehrung(); }
 const Preis GPreis() const; // Gesamtpreis
// void setVerarbeitung(const cH_Prozess p);
 const Preis EPreis(bool brutto=true) const; // Einzelpreis
 int Rabatt() const { return rabatt;}
 float PreisMenge() const { return preis.PreisMenge(); }
 void abschreiben(mengen_t menge) throw(SQLerror);
 bool allesOK() const;
 std::string Planung() const;
 ppsInstanz::ID LetztePlanInstanz() const
 {  return letztePlanInstanz; }
 // gibt Zeilennummer zurück;
 void moveInstanz(const AuftragBase& auftrag) throw(SQLerror);
 cH_Lieferschein getLieferschein() const ;
private:
 std::vector<AufEintragBase> getKundenAuftragV() const;
public:
 AufEintragBase getFirstKundenAuftrag() const;

 ArtikelBase::ID ArtId() const {return artikel.Id();}
 ArtikelBase Artikel() const {return artikel;}

// einen Teil des Auftrages=0 verplanen (in anderen Auftrag<>0 setzen)
// gibt Zeilennummer zurück; rekursiv = alle Instanzen darunter auch planen
 int Planen(mengen_t menge, const AuftragBase &zielauftrag,bool rekursiv=false);

// friend std::ostream &operator<<(std::ostream &o,const AufEintrag &aeb);
};

//std::ostream &operator<<(std::ostream &o,const AufEintrag &aeb);

#endif
