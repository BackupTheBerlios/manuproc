/* $Id: LieferscheinEntry.h,v 1.1 2001/04/23 08:11:59 christof Exp $ */
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


#ifndef LIEFERENTRY
#define LIEFERENTRY

#include<Aux/SQLerror.h>
#include<Aux/Datum.h>
#include<Artikel/ArtikelBase.h>
#include<Auftrag/AufEintragBase2.h>
#include"LieferscheinBase.h"
#include <Aux/fixedpoint.h>

class LieferscheinEntry : public LieferscheinBase
{
 int zeilennr;
 ArtikelBase artikel;
 AufEintragBase2 refauftrag;
 int stueck;
 fixedpoint<3> menge;
 int palette;
 string yourauftrag;
 bool zusatzinfo;

public:
        LieferscheinEntry(int lfrsid) throw(SQLerror);
//        vector<st_lfrsentry> get_Entrys() const {return lfrsentry; }
  

 	LieferscheinEntry() :zeilennr(0),stueck(0),palette(0) {};
 // laedt aus Datenbank
 	LieferscheinEntry(const LieferscheinBase &lsbase, int zeile) throw(SQLerror);
// erzeugen entsprechende Zeilen in der Datenbank 	
 	LieferscheinEntry(const LieferscheinBase &lsb,	
 			const AufEintragBase2 &auf,
 			const ArtikelBase &art, int anzahl, fixedpoint<3> menge,
 			int _palette=0,bool zusatzinfo=false) throw(SQLerror);
 	LieferscheinEntry(const LieferscheinBase &lsb,	
 			const ArtikelBase &art, int anzahl, fixedpoint<3> menge,
 			int _palette=0,bool zusatzinfo=false) throw(SQLerror);
// Konstruktor mit Datenbankdaten 			
        LieferscheinEntry(int l,int z,int a, int s,fixedpoint<3> m,int p,string y,bool zi)
                : LieferscheinBase(l),zeilennr(z),artikel(a),stueck(s),menge(m),
                        palette(p),yourauftrag(y),zusatzinfo(zi) {};
 fixedpoint<3> Menge() const { return menge; }
 int Anzahl() const { return stueck; }
 int Stueck() const { return stueck; }
 int Palette() const { return palette; }
 const string YourAuftrag() const {return yourauftrag; }
 const ArtikelBase::ID ArtikelID() const { return artikel.Id(); }
 int AufId() const { return refauftrag.Id();} 
 int AufZeile() const { return refauftrag.ZNr();} 
 int Zeile() const { return zeilennr; }
 bool ZusatzInfo() const { return zusatzinfo; }
 static void LieferscheinEntry::deleteEntry(const LieferscheinEntry &lse) throw(SQLerror);
};

#endif
