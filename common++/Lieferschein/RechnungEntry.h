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

#include<Aux/SQLerror.h>
#include<Aux/Datum.h>
#include<Artikel/ArtikelBase.h>
#include<Auftrag/AufEintragBase.h>
#include<Artikel/Preis.h>
#include"RechnungBase.h"
#include"LieferscheinEntry.h"

class RechnungEntryBase : public RechnungBase
{
public:
  typedef RechnungBase::rabatt_t rabatt_t;
  typedef RechnungBase::geldbetrag_t geldbetrag_t;
  typedef RechnungBase::mengen_t mengen_t;
protected:
 int zeilennr;
 
public:
	RechnungEntryBase() : zeilennr(0) {}
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
 Petig::Datum lieferdatum;
 LieferscheinEntryBase lfrs;
// LieferscheinBase::ID lfrsid;
// int lieferzeile;
 
public:

 RechnungEntry(const RechnungEntryBase &reb);
 RechnungEntry(const RechnungBase &l, int z,int a, int s,mengen_t m,const Preis &p, rabatt_t r,
                        Petig::Datum ld, const LieferscheinEntryBase ls)
  		: RechnungEntryBase(l,z),artikel(a),stueck(s),menge(m),preis(p),
                        rabatt(r),lieferdatum(ld),lfrs(ls)
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
   else return preis.Gesamtpreis(1,0,rabatt); 
 }
 
 // Waehrung muss zu der in Rechnung passen (kein Vergleich)
 void setzePreis(const Preis &p) throw (SQLerror);

 rabatt_t Rabatt() const { return rabatt;}

 const ArtikelBase::ID ArtikelID() const { return artikel.Id(); }
 const ArtikelBase Artikel() const { return artikel; }
 Petig::Datum LieferDatum() const {return lieferdatum; }
 const Preis GPreis() const { return preis.Gesamtpreis(stueck,menge,rabatt); }
};

#endif
