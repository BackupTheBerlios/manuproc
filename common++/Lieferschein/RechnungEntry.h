/* $Id: RechnungEntry.h,v 1.25 2004/01/28 14:52:55 jacek Exp $ */
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


#ifndef RECHNUNGENTRY__
#define RECHNUNGENTRY__

#include<Misc/SQLerror.h>
#include<Misc/Datum.h>
#include<Artikel/ArtikelBase.h>
#include<Auftrag/AufEintragBase.h>
#include<Artikel/Preis.h>
#include"RechnungBase.h"
#include"LieferscheinEntry.h"
#include <BaseObjects/ManuProcEintrag.h>
#include <map>




class RechnungEntryBase : public RechnungBase
{
public:
  typedef RechnungBase::rabatt_t rabatt_t;
  typedef RechnungBase::geldbetrag_t geldbetrag_t;
  typedef RechnungBase::mengen_t mengen_t;
  static const int none_znr=ManuProcEintrag::none_znr;

protected:
 int zeilennr;
 
 struct rng_payoff {
 	bool valid;
 	ManuProC::Datum rgdatum;
 	bool bezahlt;
 	cH_Kunde kunde;
 	RngArt rngart;
 	rng_payoff(const ManuProC::Datum d, bool b, cH_Kunde k,RngArt a)
 		:valid(true),rgdatum(d),bezahlt(b),kunde(k),rngart(a) {}
 	rng_payoff():valid(false),bezahlt(false),kunde(Kunde::none_id),
 			rngart(RART_NONE){}
 	};

  static std::map<int,struct rng_payoff> rgdata;
  static void setRgPayOff(int _rngid) throw(SQLerror);
  
public:
	RechnungEntryBase() : zeilennr(none_znr) {}
	RechnungEntryBase(const RechnungBase &rb, int z)
	: RechnungBase(rb), zeilennr(z) {}

	int Zeile() const { return zeilennr; }

 static void deleteEntry(const RechnungEntryBase &re) throw(SQLerror); 
};

class RechnungEntry : public RechnungEntryBase
{
 ArtikelBase artikel;
 AufEintragBase refauftrag;
 int stueck;
 mengen_t menge;
 Preis preis;
 rabatt_t rabatt;
 ManuProC::Datum lieferdatum;
 LieferscheinEntryBase lfrs;
 AuftragBase auftrag;
// LieferscheinBase::ID lfrsid;
// int lieferzeile;
 
public:

 RechnungEntry(const RechnungEntryBase &reb) throw(SQLerror);
 RechnungEntry(const RechnungBase &l, int z,int a, int s,mengen_t m,const Preis &p, rabatt_t r,
                        ManuProC::Datum ld, const LieferscheinEntryBase ls,
                        const AuftragBase &ab)
  		: RechnungEntryBase(l,z),artikel(a),stueck(s),menge(m),preis(p),
                        rabatt(r),lieferdatum(ld),lfrs(ls),auftrag(ab)
                {};

 mengen_t Menge() const { return menge; }
 int Stueck() const { return stueck; }

// teuer !!!
 LieferscheinEntry LfrsEntry() const 
   { return LieferscheinEntry(lfrs); }

 LieferscheinEntryBase Lfrs() const { return lfrs; }

 Preis getAuftragsPreis();

 const Preis getPreis(bool brutto=true) const 
 { 
   if(brutto) return preis;
#warning   
#warning I think, it should be 1 for stueck here and not 0, but beware !   
   else return Preis(preis.Gesamtpreis(preis.getWaehrung(),
   			preis.PreisMenge().as_int(),0.0,rabatt),
   			preis.getWaehrung(),preis.PreisMenge());
 }
 
 // Waehrung muss zu der in Rechnung passen (kein Vergleich)
 void setzePreis(const Preis &p) throw (SQLerror);

 rabatt_t Rabatt() const { return rabatt;}

 const ArtikelBase::ID ArtikelID() const { return artikel.Id(); }
 const ArtikelBase Artikel() const { return artikel; }
 ManuProC::Datum LieferDatum() const {return lieferdatum; }
 const ManuProC::Datum RgDatum() const throw(SQLerror);
 const bool Bezahlt() const throw(SQLerror);
 cH_Kunde Kunde() const;  
 const RngArt RgArt() const throw(SQLerror);
 const int AuftragId() const { return auftrag.Id();}
 const Preis GPreis() const { 
 	return Preis(preis.Gesamtpreis(preis.getWaehrung(),
 				stueck,menge.as_float(),rabatt),
 			preis.getWaehrung()); }
 
};



#endif
