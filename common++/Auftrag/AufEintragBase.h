/* $Id: AufEintragBase.h,v 1.21 2002/01/22 09:15:55 christof Exp $ */
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
#include <Auftrag/AufEintragBase2.h>
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


class AufEintragBase : public AufEintragBase2
{
// int Id() const; //Use ArtId()
 
protected: 
 ArtikelBase artikel;

 int dispoentrynr;
 int disponr;
 
 AufStatVal auftragstatus; /* Status des zugehörigen Auftrages */
 AufStatVal entrystatus; /* Status des zugehörigen Eintrags */
 
 int kdnr;
 std::string youraufnr;
 mengen_t bestellt;    /* Stück */
 mengen_t geliefert;   /* Stück */
 Petig::Datum lieferdatum;
 Petig::Datum lasteditdate;
 cH_Prozess prozess;
 int letztePlanInstanz,maxPlanInstanz;
 Petig::Datum prozdate;

 Preis preis;
 int rabatt;     // % * 100
 	
public:
 struct Error : public std::exception
   {  virtual const char* what() const throw() { return "AufEintragBase::AufEintragBaseError"; }
       Error() {}
   };
 struct NoAEB_Error : public std::exception
   {  virtual const char* what() const throw() { return "AufEintragBase::NoAEB_Error"; }
       NoAEB_Error() {}
   };

 AufEintragBase() 
   : artikel(0ul), dispoentrynr(0),
   	disponr(0), auftragstatus((AufStatVal)UNCOMMITED), entrystatus((AufStatVal)UNCOMMITED),
   	kdnr(0), 
   	bestellt(0),
   	geliefert(0), prozess(Prozess::default_id),
   	letztePlanInstanz(0),maxPlanInstanz(0),
	rabatt(0)
 {}

 AufEintragBase(ppsInstanz::ID _instanz,int _auftragid, int _zeilennr, 
        mengen_t _bestellt,
	int _artikel, const Petig::Datum _lieferdatum,
	mengen_t _geliefert,
	int _dispoentrynr, int _disponr, 
	AufStatVal _aufstatus,
	int _kdnr, const std::string _youraufnr,
	const Petig::Datum& _prozdate,
	int _prozess,int _letztePlanInstanz, int _maxPlanInstanz,
	const Preis &_preis, int _rabatt,
	AufStatVal _entrystat, const Petig::Datum _lasteditdate) throw();
 AufEintragBase(const AufEintragBase2 &aebb) throw (SQLerror,NoAEB_Error);

	
 void updateDispoENr(int dinr) throw(SQLerror);
 void updateStk(mengen_t stk,bool instanz) throw(SQLerror);
 void updateStkInstanz(mengen_t neu_stk,mengen_t alt_stk) throw(SQLerror);
 void updateLieferdatum(const Petig::Datum &ld) throw(SQLerror);	
 void updateLieferdatum(const Kalenderwoche &K) {updateLieferdatum(Petig::Datum(K));}	
 void updateLieferdatumInstanz(const Petig::Datum &ld) throw(SQLerror);	
 void updatePreis(const Preis &pr) throw(SQLerror);
 void updateRabatt(int rb) throw(SQLerror);
 void setStatus(AufStatVal newstatus) throw(SQLerror);		
 void setInstanzen(AufStatVal newstatus,Petig::Datum lieferdate,mengen_t part,int myznr=-1,int yourznr=-1);
 void split(mengen_t newmenge, const Petig::Datum &newld) throw(SQLerror);
// void setPlanMeter(mengen_t gm) { geplante_menge=gm; }
 
// long getMeter() const { return (long)menge; } 
// long getPlanMeter() const { return geplante_menge; }
// mengen_t getRest() const { if(entrystatus==CLOSED)return 0; return (mengen_t)rest; }	
 mengen_t getStueck() const { return bestellt;}
 mengen_t getRestStk() const {if(entrystatus==CLOSED)return 0; return bestellt-geliefert;}
 mengen_t getGeliefert() const { return geliefert;}
//A mengen_t getGeliefertM() const { return mengen_t(
//A 		(geliefert*(artikel->Stueckgroesse()*10.0))/10);}
// const char *getLastDate() const { return lasteditdate.c_str();}
// const char *getLastEdit() const { return lastedit.c_str(); }
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
// void calculateProzessInstanz();
// std::vector<pair<cH_Prozess,long> > getProzess2() const;
// std::string getProzess2_c_str() const;
 const cP_Waehrung getWaehrung() const { return preis.getWaehrung(); }
 const Preis GPreis() const; // Gesamtpreis
 void setVerarbeitung(const cH_Prozess p);
 const Preis EPreis() const { return preis;} // Einzelpreis
 int Rabatt() const { return rabatt;}
 float PreisMenge() const { return preis.PreisMenge(); }
 void abschreiben(mengen_t menge) throw(SQLerror);
 bool allesOK() const;
 std::string Planung() const;

 ArtikelBase::ID ArtId() const {return artikel.Id();}
 ArtikelBase Artikel() const {return artikel;}


// void setArtikelBezeichnung(const cH_ExtBezSchema &cs)
// 	{const_cast<AufArtikel&>(*artikel).setArtikelBezeichnung(cs); } 
  
 friend std::ostream &operator<<(std::ostream &o,const AufEintragBase &aeb);
};

std::ostream &operator<<(std::ostream &o,const AufEintragBase &aeb);

#endif
