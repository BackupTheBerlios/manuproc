/* $Id: LieferscheinEntry.h,v 1.21 2003/07/03 06:47:10 christof Exp $ */
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

#include <Misc/SQLerror.h>
#include <Artikel/ArtikelBase.h>
#include <Auftrag/AufEintragBase.h>
#include <Lieferschein/LieferscheinBase.h>
#include <Misc/fixedpoint.h>
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
	LieferscheinEntryBase(cH_ppsInstanz inst, int ls, int znr)
	: LieferscheinBase(inst,ls), zeilennr(znr) {}

 int Zeile() const { return zeilennr; }
 int ZNr() const { return zeilennr; }
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
public:
 struct st_zusatz{AufEintragBase aeb; AuftragBase::mengen_t menge;std::string yourauftrag;
        st_zusatz()  {};
        st_zusatz(AufEintragBase a, AuftragBase::mengen_t m,std::string y) 
         : aeb(a),menge(m),yourauftrag(y) {}
        };
private:
 std::vector<st_zusatz> VZusatz;

   void updateLieferscheinMenge(int stueck,mengen_t menge)  throw(SQLerror);
   mengen_t Abschreibmenge(int stueck,mengen_t menge) const;

public:

 	LieferscheinEntry() : stueck(0),palette(0) {};
 	LieferscheinEntry(const LieferscheinEntryBase &lsbase) throw(SQLerror);
private:
  friend class Lieferschein;
  static LieferscheinEntry create(const LieferscheinBase &lsb,
           const AufEintragBase &auf,
           const ArtikelBase &art, int anzahl, mengen_t menge,
           int _palette=0,bool zusatzinfo=false) throw(SQLerror);
  static LieferscheinEntry create(const LieferscheinBase &lsb,
           const ArtikelBase &art, int anzahl, mengen_t menge,
           int _palette=0,bool zusatzinfo=false) throw(SQLerror);

public:
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
 std::vector<st_zusatz> getZusatzInfos() const {return VZusatz;}
 void setZusatzInfo(const AufEintragBase &AEB,/*const int stueck,*/const mengen_t &menge) throw(SQLerror);

 void setPalette(int p) throw(SQLerror);
 bool changeMenge(int stueck,mengen_t menge) throw(SQLerror);
 static void deleteEntry(LieferscheinEntry &lse) throw(SQLerror);
private:
 static void deleteMe(const LieferscheinEntry &lse) throw(SQLerror);
 void deleteZusatzEntry(const st_zusatz &Z) throw(SQLerror);
 void updateZusatzEntry(const st_zusatz &Z,const AuftragBase::mengen_t &menge) throw(SQLerror);
 friend FetchIStream& operator>>(FetchIStream& is,LieferscheinEntry &aeb);
};

class ArgumentList;
ArgumentList& operator<<(ArgumentList &,const LieferscheinEntryBase &aeb);

#endif
