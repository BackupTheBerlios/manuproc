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
#include<Auftrag/AufEintragBase2.h>
#include<Artikel/Preis.h>
#include"RechnungBase.h"
#include"LieferscheinBase.h"

class RechnungEntry : protected RechnungBase
{
 int zeilennr;
 ArtikelBase artikel;
 AufEintragBase2 refauftrag;
 int stueck;
 float menge;
 Preis preis;
 fixedpoint<2> rabatt;
 Petig::Datum lieferdatum;
 LieferscheinBase::ID lfrsid;
 int lieferzeile;
 
public:
  
 RechnungEntry(int l, int z,int a, int s,float m,const Preis &p, float r,
                        Petig::Datum ld, int lid,int lifz)
  		: RechnungBase(l),zeilennr(z),artikel(a),stueck(s),menge(m),preis(p),
                        rabatt(r),lieferdatum(ld),lfrsid(lid),
                        lieferzeile(lifz)
                {};

 float Menge() const { return menge; }
 int Stueck() const { return stueck; }
 LieferscheinBase::ID Lfrs_Id() const { return lfrsid; }
 int Lfrs_ZNr() const { return lieferzeile; }
 const Preis getPreis() const { return preis;}
 // Waehrung muss zu der in Rechnung passen (kein Vergleich)
 void setzePreis(const Preis &p) throw (SQLerror);
 fixedpoint<2> Rabatt() const { return rabatt;}
 float PreisMenge() const {return preis.PreisMenge();}
 const ArtikelBase::ID ArtikelID() const { return artikel.Id(); }
 Petig::Datum LieferDatum() const {return lieferdatum; }
 int Zeile() const { return zeilennr; }
 const Preis GPreis() const { return preis.Gesamtpreis(stueck,menge,rabatt); }
 static void deleteEntry(const RechnungEntry &re) throw(SQLerror); 
};

#endif
