/* $Id: LieferscheinEntry.h,v 1.7 2002/02/05 17:15:52 christof Exp $ */
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
#include<Artikel/ArtikelBase.h>
#include<Auftrag/AufEintragBase.h>
#include"LieferscheinBase.h"
#include <Aux/fixedpoint.h>

class LieferscheinEntry : public LieferscheinBase
{
 int zeilennr;
 ArtikelBase artikel;
 AufEintragBase refauftrag;
 int stueck;
 mengen_t menge;
 int palette;
 std::string yourauftrag;
 bool zusatzinfo;

public:
//???MAT        LieferscheinEntry(int lfrsid) throw(SQLerror);
//        std::vector<st_lfrsentry> get_Entrys() const {return lfrsentry; }
  

 	LieferscheinEntry() :zeilennr(0),stueck(0),palette(0) {};
 // laedt aus Datenbank
 	LieferscheinEntry(const LieferscheinBase &lsbase, int zeile) throw(SQLerror);
// erzeugen entsprechende Zeilen in der Datenbank 	
 	LieferscheinEntry(const LieferscheinBase &lsb,	
 			const AufEintragBase &auf,
 			const ArtikelBase &art, int anzahl, mengen_t menge,
 			int _palette=0,bool zusatzinfo=false) throw(SQLerror);
 	LieferscheinEntry(const LieferscheinBase &lsb,	
 			const ArtikelBase &art, int anzahl, mengen_t menge,
 			int _palette=0,bool zusatzinfo=false) throw(SQLerror);
// Konstruktor mit Datenbankdaten 			
        LieferscheinEntry(const cH_ppsInstanz& _instanz,int l,int z,int a, int s,mengen_t m,int p,
        		const std::string &y,bool zi,const AufEintragBase &aeb)
                : LieferscheinBase(_instanz,l),zeilennr(z),artikel(a),refauftrag(aeb),
                	stueck(s),menge(m),palette(p),yourauftrag(y),
                	zusatzinfo(zi) 
                {};
 mengen_t Menge() const { return menge; }
 int Anzahl() const { return stueck; }
 int Stueck() const { return stueck; }
 int Palette() const { return palette; }
 const std::string YourAuftrag() const {return yourauftrag; }
 const ArtikelBase::ID ArtikelID() const { return artikel.Id(); }
 int AufId() const { return refauftrag.Id();} 
 int AufZeile() const { return refauftrag.ZNr();} 
 int Zeile() const { return zeilennr; }
 bool ZusatzInfo() const { return zusatzinfo; }
 static void LieferscheinEntry::deleteEntry(const LieferscheinEntry &lse) throw(SQLerror);
};

#endif
