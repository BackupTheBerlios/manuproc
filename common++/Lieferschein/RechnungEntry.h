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
#include"LieferscheinBase.h"

class RechnungEntry : protected RechnungBase
{
public:
  typedef RechnungBase::rabatt_t rabatt_t;
  typedef RechnungBase::geldbetrag_t geldbetrag_t;
  typedef RechnungBase::mengen_t mengen_t;

private:
 int zeilennr;
 ArtikelBase artikel;
 AufEintragBase refauftrag;
 int stueck;
 mengen_t menge;
 Preis preis;
 rabatt_t rabatt;
 Petig::Datum lieferdatum;
 LieferscheinBase::ID lfrsid;
 int lieferzeile;
 
public:

 RechnungEntry(const RechnungBase& rechnung);  
 RechnungEntry(int l, int z,int a, int s,mengen_t m,const Preis &p, rabatt_t r,
                        Petig::Datum ld, int lid,int lifz)
  		: RechnungBase(l),zeilennr(z),artikel(a),stueck(s),menge(m),preis(p),
                        rabatt(r),lieferdatum(ld),lfrsid(lid),
                        lieferzeile(lifz)
                {};

 mengen_t Menge() const { return menge; }
 int Stueck() const { return stueck; }
 LieferscheinBase::ID Lfrs_Id() const { return lfrsid; }
 int Lfrs_ZNr() const { return lieferzeile; }
 const Preis getPreis() const { return preis;}
 // Waehrung muss zu der in Rechnung passen (kein Vergleich)
 void setzePreis(const Preis &p) throw (SQLerror);
 rabatt_t Rabatt() const { return rabatt;}
 Preis::preismenge_t PreisMenge() const {return preis.PreisMenge();}
 const ArtikelBase::ID ArtikelID() const { return artikel.Id(); }
 Petig::Datum LieferDatum() const {return lieferdatum; }
 int Zeile() const { return zeilennr; }
 const Preis GPreis() const { return preis.Gesamtpreis(stueck,menge,rabatt); }
 static void deleteEntry(const RechnungEntry &re) throw(SQLerror); 
};

#endif
