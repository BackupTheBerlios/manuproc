/* $Id: Lieferschein.h,v 1.22 2002/11/29 11:29:29 thoma Exp $ */
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

#ifndef LIEFERSCHEIN
#define LIEFERSCHEIN

#include"LieferscheinBase.h"
#include"LieferscheinEntry.h"
#include<Aux/SQLerror.h>
#include<Aux/Datum.h>
#include<Kunde/Kunde.h>
#include <vector>
#include <Aux/Handles.h>
#include <Aux/CacheStatic.h>
#include <Artikel/Preis.h>
#include <Auftrag/AufEintrag.h>
#include <DynamicEnums/DefaultValues.h>

class Lieferschein : public LieferscheinBase, public HandleContent
{
 ManuProC::Datum lsdatum;
 cH_Kunde kunde;
 int rngid;
 ManuProC::Datum geliefertam;
#ifdef DPD_LIEFERSCHEINE
 int dpdliefnr;
 int paeckchen, pakete;
 typedef fixedpoint<ManuProC::Precision::LieferscheinGewicht> gewicht_t;
 gewicht_t brutto_kg,netto_kg;
#endif
 public:
        
 	Lieferschein(const LieferscheinBase &source)
 		: LieferscheinBase(source), kunde(Kunde::none_id)  {}
 		
 	Lieferschein() : LieferscheinBase(),
 			lsdatum(ManuProC::Datum::today()),
 			kunde(Kunde::none_id),rngid(ManuProcEntity<>::none_id),
 			geliefertam(ManuProC::Datum::today())
#ifdef DPD_LIEFERSCHEINE
 			,dpdliefnr(0) ,
 			paeckchen(0),pakete(0)
#endif
 			{}
 			
 	Lieferschein(const cH_ppsInstanz& instanz,int lid) throw(SQLerror);
	Lieferschein(const LieferscheinBase &lsbase,
			const ManuProC::Datum &_lsdatum,
			int _kdnr, int _rngid, int _paeckchen, int _pakete,
			const ManuProC::Datum &_geliefertam,int _dpdlnr); 
			
 Lieferschein(const cH_ppsInstanz& instanz,cH_Kunde k,int jahr=0) throw(SQLerror);
 void setDPDDatum() const throw(SQLerror);
			
#ifdef DPD_LIEFERSCHEINE
 const static int Fertig=0;
 const static int Offen=-1;
 int getDPDlnr() const {return dpdliefnr;}
 void setDPDlnr(int dpdlnr) const throw(SQLerror);
 int Pakete() const {return pakete;}			
 int Paeckchen() const {return paeckchen;}
 void setPakete(const int i) throw(SQLerror);
 void setPaeckchen(const int i) throw(SQLerror);
 gewicht_t GewichtBrutto() const {return brutto_kg;}
 gewicht_t GewichtNetto() const {return netto_kg;}
 void setGewichtBrutto(const gewicht_t i) throw(SQLerror);
 void setGewichtNetto(const gewicht_t i) throw(SQLerror);
 const ManuProC::Datum getMaxZahlziel() const throw(SQLerror);
#endif 
 const Preis::rabatt_t AufRabatt() const throw(SQLerror);
 
 int KdNr() const {return kunde->Id();}		
 const cH_Kunde &getKunde() const {return kunde; }	
 int RngNr() const { return rngid; }
 void closeLfrs(); // setzt rngid auf 0  
 ExtBezSchema::ID getSchema() const { return kunde->getSchemaId(); }
 int maxZnr() throw(SQLerror);
 const ManuProC::Datum LsDatum() const { return lsdatum; }
 const ManuProC::Datum getDatum() const { return geliefertam; }
 void setDatum(const ManuProC::Datum &d) throw(SQLerror);
 // DB Zugriff mit abschreiben
 void push_back(AufEintrag &auftragentry, 
 		const ArtikelBase &artikel, int anzahl, mengen_t menge, int palette);
 // DB: Menge verteilt auf mehrere Aufträge abschreiben
 void push_back(const ArtikelBase &artikel, int anzahl, mengen_t menge, int palette);


 // Rechnungsnummer von Lieferscheinen ohne Entrys auf '0' setzen
 static void aufraumen() throw(SQLerror);
 static mengen_t StandardLaenge(const ArtikelBase artikel);

};

class cH_Lieferschein : public Handle<const Lieferschein>
{
 friend class std::map<int,cH_Lieferschein> ;
protected:
 cH_Lieferschein() {}
public:
 cH_Lieferschein(Lieferschein *r) : Handle<const Lieferschein>(r){}
 cH_Lieferschein(const cH_ppsInstanz& instanz,Lieferschein::ID i) 
        : Handle<const Lieferschein>(new Lieferschein(instanz,i)){}
};

class H_Lieferschein : public Handle<Lieferschein>
{
 friend class std::map<int,H_Lieferschein> ;
protected:
public:
 H_Lieferschein(): Handle<Lieferschein> (new Lieferschein()) {}
 H_Lieferschein(Lieferschein *r) : Handle<Lieferschein>(r){}
 H_Lieferschein(const cH_ppsInstanz& instanz,Lieferschein::ID i) 
     : Handle<Lieferschein>(new Lieferschein(instanz,i)){}
};

/*
class cached_Lieferschein : public cH_Lieferschein
{
   cH_Lieferschein lieferschein;

   typedef CacheStatic<int,cH_Lieferschein> cache_t;
   static cache_t cache;
 public:
   cached_Lieferschein(int lid) ;
};


cached_Lieferschein::cache_t cached_Lieferschein::cache;

cached_Lieferschein::cached_Lieferschein(int lid)
{
      cH_Lieferschein *cached(cache.lookup(lid));
      if (cached) { lieferschein = *cached; return; }
      lieferschein = new Lieferschein(lid);
      
      cache.Register(lid,lieferschein);
}
*/
#endif
