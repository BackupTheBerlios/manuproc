/* $Id: LieferscheinEntry.h,v 1.13 2002/09/02 13:04:04 christof Exp $ */
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
#include <BaseObjects/ManuProcEintrag.h>

class LieferscheinEntryBase : public LieferscheinBase
{
public: 
   static const int none_znr = ManuProcEintrag::none_znr;
 protected:
	int zeilennr;

	LieferscheinEntryBase(const LieferscheinBase &lsb)
	: LieferscheinBase(lsb), zeilennr(none_znr) {}
public:
	LieferscheinEntryBase() : zeilennr(none_znr) {}
	LieferscheinEntryBase(const LieferscheinBase &lsb, int znr)
	: LieferscheinBase(lsb), zeilennr(znr) {}
	// unschön, da Ints ...
	LieferscheinEntryBase(ppsInstanz::ID inst, int ls, int znr)
	: LieferscheinBase(inst,ls), zeilennr(znr) {}

 int Zeile() const { return zeilennr; }
};

class LieferscheinEntry : public LieferscheinEntryBase
{
 ArtikelBase artikel;
 AufEintragBase refauftrag;
 int stueck;
 mengen_t menge;
 int palette;
 std::string yourauftrag;
 bool zusatzinfo;

   void updateLieferscheinMenge(int stueck,mengen_t menge)  throw(SQLerror);
   bool menge_bei_zusatzinfos_abschreiben(std::vector<LieferscheinEntry>& VLE,mengen_t menge);
   mengen_t Abschreibmenge(int stueck,mengen_t menge) const;

public:

 	LieferscheinEntry() : stueck(0),palette(0) {};
 // laedt aus Datenbank
 	LieferscheinEntry(const LieferscheinEntryBase &lsbase) throw(SQLerror);
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
                : LieferscheinEntryBase(LieferscheinBase(_instanz,l),z),
                	artikel(a),refauftrag(aeb),
                	stueck(s),menge(m),palette(p),yourauftrag(y),
                	zusatzinfo(zi) 
                {};
 bool Valid() const;
 mengen_t Menge() const { return menge; }
 int Anzahl() const { return stueck; }
 int Stueck() const { return stueck; }
 int Palette() const { return palette; }
 const std::string YourAuftrag() const {return yourauftrag; }
 const ArtikelBase::ID ArtikelID() const { return artikel.Id(); }
 const ArtikelBase Artikel() const { return artikel; }
 const AuftragBase RefAuftrag() const { return refauftrag; }
 int AufZeile() const { return refauftrag.ZNr();} 
 AufEintragBase getAufEintragBase() const {return AufEintragBase(RefAuftrag(),AufZeile());}
 bool ZusatzInfo() const { return zusatzinfo; }

 void setPalette(int p) throw(SQLerror);
 bool changeMenge(int stueck,mengen_t menge) throw(SQLerror);
 static void deleteEntry(const LieferscheinEntry &lse) throw(SQLerror);
};

#endif
