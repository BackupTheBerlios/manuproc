/* $Id: AufEintragBase.h,v 1.16 2001/12/04 08:42:10 christof Exp $ */
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
 
 AufStatVal status; /* Status des zugehörigen Auftrages */
 AufStatVal entrystatus; /* Status des zugehörigen Eintrags */
 
 int kdnr;
 std::string youraufnr;
 long bestellt;    /* Stück */
 long geliefert;   /* Stück */
// double rest;        /* Meter */
// double menge;        /* Meter */
// long geplante_menge;
 Petig::Datum lieferdatum;
 Petig::Datum lasteditdate;
 int jahrgang; // soll bald weg
 cH_Prozess prozess;
 int letztePlanInstanz,maxPlanInstanz;
 Petig::Datum prozdate;

 Preis preis;
 int rabatt;     // % * 100
 	
public:
 AufEintragBase() 
   : artikel(0ul), dispoentrynr(0),
   	disponr(0), status((AufStatVal)OPEN), entrystatus((AufStatVal)OPEN),
   	kdnr(0), 
   	bestellt(0),
   	geliefert(0), jahrgang(0), prozess(Prozess::default_id),
   	letztePlanInstanz(0),maxPlanInstanz(0),
	rabatt(0)
 {}

 AufEintragBase(ppsInstanz::ID _instanz,int _auftragid, int _zeilennr, int _bestellt,
	int _artikel, const Petig::Datum _lieferdatum,
	int _geliefert,
	int _dispoentrynr, int _disponr, int _jahrgang,
	AufStatVal _aufstatus,
	int _kdnr, const std::string _youraufnr,
	const Petig::Datum& _prozdate,
	int _prozess,int _letztePlanInstanz, int _maxPlanInstanz,
	const Preis &_preis, int _rabatt,
	AufStatVal _entrystat, const Petig::Datum _lasteditdate) throw();
 AufEintragBase(const AufEintragBase2 &aebb) throw (SQLerror);
	
 void updateDispoENr(int dinr) throw(SQLerror);
 void updateStk(long stk) throw(SQLerror);
 void updateLieferdatum(const Petig::Datum &ld) throw(SQLerror);	
 void updateLieferdatum(const Kalenderwoche &K) {updateLieferdatum(Petig::Datum(K));}	
 void updatePreis(const Preis &pr) throw(SQLerror);
 void updateRabatt(int rb) throw(SQLerror);
 void setStatus(AufStatVal st) throw(SQLerror);		
 void split(int newmenge, const Petig::Datum &newld) throw(SQLerror);
// void setPlanMeter(long gm) { geplante_menge=gm; }
 
// long getMeter() const { return (long)menge; } 
// long getPlanMeter() const { return geplante_menge; }
// long getRest() const { if(entrystatus==CLOSED)return 0; return (long)rest; }	
 long getStueck() const { return bestellt;}
 long getRestStk() const {if(entrystatus==CLOSED)return 0; return bestellt-geliefert;}
 long getGeliefert() const { return geliefert;}
//A long getGeliefertM() const { return long(
//A 		(geliefert*(artikel->Stueckgroesse()*10.0))/10);}
// const char *getLastDate() const { return lasteditdate.c_str();}
// const char *getLastEdit() const { return lastedit.c_str(); }
 AufStatVal getAufStatus() const { return status; }
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
 int getJahrgang() const { return jahrgang;}
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
 void abschreiben(int menge) throw(SQLerror);
 bool allesOK() const;
 std::string Planung() const;
/*
 struct st_reflist {AufEintragBase2 AEB2;ArtikelBase AB;long Menge;
        st_reflist(AufEintragBase2 aeb2,ArtikelBase ab,long menge)
           :AEB2(aeb2),AB(ab),Menge(menge){}};
private:
 list<cH_Kunde> get_Referenz_Kunden_long() const throw(SQLerror);
public:
 // Eine Benachbarte Liste von Kind- bzw. Elternaufträgen:
 list<st_reflist> get_Referenz_list(const AufEintragBase2& aeb,bool kinder=false) const throw(SQLerror);
 // Für einen KOMPLETTEN Auftragsbaum bitte die Klasse AuftragsBaum verwenden
 // die folgende Funktion liefert nur die Endaufträge OHNE Knoten
 list<st_reflist> get_Referenz_listFull(bool kinder) const throw(SQLerror);
                //kinder=false:   Elternaufträge 
                //kinder=true:    Kinderaufträge
*/
/*
 list<st_reflist> get_Referenz_AufEintragBase2(bool ursprung=true,bool kinder=false) const throw(SQLerror);
                //Ursprung=false: Nur die benachbarten Aufträge 
                //Ursprung=true:  Alle (Referenz-)Aufträge 
                //kinder=false:   Elternaufträge 
                //kinder=true:    Kinderaufträge
*/
// long get_Referenz_AufEintragBase2_Summe(int instanz,bool ursprung=true,bool kinder=false) const throw(SQLerror);
// list<cH_Kunde> get_Referenz_Kunden() const throw(SQLerror);
 ArtikelBase::ID ArtId() const {return artikel.Id();}

/*
 static std::list<AufEintragBase2> get_AufEintragList_from_Artikel
               (const ArtikelBase& artikel,const cH_ppsInstanz& instanz);
 static AufEintragBase2 get_AufEintrag_from_Artikel_by_Lfdate
               (const ArtikelBase& artikel,const cH_ppsInstanz& instanz); 
*/  

// void setArtikelBezeichnung(const cH_ExtBezSchema &cs)
// 	{const_cast<AufArtikel&>(*artikel).setArtikelBezeichnung(cs); } 
  
 friend std::ostream &operator<<(std::ostream &o,const AufEintragBase &aeb);
};

std::ostream &operator<<(std::ostream &o,const AufEintragBase &aeb);

#endif
