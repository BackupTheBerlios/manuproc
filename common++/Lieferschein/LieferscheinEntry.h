/* $Id: LieferscheinEntry.h,v 1.36 2004/02/19 15:59:32 jacek Exp $ */
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


#ifndef LIEFERENTRY_H
#define LIEFERENTRY_H

#include <Lieferschein/LieferscheinEntryBase.h>
#include <Misc/SQLerror.h>
#include <Artikel/ArtikelBase.h>
#include <Auftrag/AufEintragBase.h>
#include <Misc/fixedpoint.h>
#include <Misc/compiler_ports.h>
#include <Auftrag/auftrag_status.h>
#include <Lager/FertigWarenLager.h>

class Lieferschein;

class LieferscheinEntry : public LieferscheinEntryBase
{
 ArtikelBase artikel;
 int stueck;
 mengen_t menge;
 int palette;
 AufStatVal status;
 mutable Kunde::ID kid;
 
public:
 struct st_AufEintragMenge{AufEintragBase aeb; AuftragBase::mengen_t menge;
        st_AufEintragMenge()  {};
        st_AufEintragMenge(AufEintragBase a, AuftragBase::mengen_t m) 
         : aeb(a),menge(m) {}
        };
 struct st_AuftragMenge{AuftragBase ab; AuftragBase::mengen_t menge;
        st_AuftragMenge()  {};
        st_AuftragMenge(AuftragBase a, AuftragBase::mengen_t m) 
         : ab(a),menge(m) {}
        };
 typedef st_AufEintragMenge st_zusatz;
 typedef std::vector<st_AufEintragMenge> zusaetze_t;

private:
 zusaetze_t VZusatz;

   void updateLieferscheinMenge(int stueck,mengen_t menge)  throw(SQLerror);
   mengen_t Abschreibmenge(int stueck,mengen_t menge) const;

public:
 	LieferscheinEntry() : stueck(0),palette(0),
 		status((AufStatVal)NOSTAT), kid(Kunde::none_id) {};
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
 const ArtikelBase::ID ArtikelID() const { return artikel.Id(); }
 const ArtikelBase Artikel() const { return artikel; }
 const AufStatVal Status() const { return status; }
 
 const zusaetze_t &getZusatzInfos() const {return VZusatz;}
 // nach Aufträgen summiert (nicht nach Zeilen getrennt)
 std::vector<st_AuftragMenge> getAuftragsMenge() const;
 void setZusatzInfos(const zusaetze_t &zis);
 void setZusatzInfos();

 __deprecated void setZusatzInfo(const AufEintragBase &AEB,const mengen_t &menge) throw(SQLerror)
 {  addZusatzEntry(AEB,menge); }

 void setPalette(int p) throw(SQLerror);
 void changeMenge(int stueck,mengen_t menge, bool ein_auftrag) throw(SQLerror);
// void changeMenge(int stueck,mengen_t menge) throw(SQLerror); 
 void changeStatus(AufStatVal new_status, 
 		bool ein_auftrag, int _stk, mengen_t _mng) 
 				throw(SQLerror,LagerError);
 void changeStatus(AufStatVal new_status, bool ein_auftrag);
 static void deleteEntry(LieferscheinEntry &lse) throw(SQLerror);
 void setLagerid(int _lagid) throw(SQLerror);

 Kunde::ID KdID() const throw(SQLerror);

 // bitte getZusatzInfos() nehmen, es könnten mehrere sein
#if 0 
 __deprecated const AuftragBase RefAuftrag() const { return VZusatz.at(0).aeb; }
 __deprecated int AufZeile() const { return VZusatz.at(0).aeb.ZNr();}
 __deprecated AufEintragBase getAufEintragBase() const {return VZusatz.at(0).aeb; }
 __deprecated bool ZusatzInfo() const { return true; }
#endif 
 
private:
 static void deleteMe(const LieferscheinEntry &lse) throw(SQLerror);
 void deleteZusatzEntry(const AufEintragBase &Z) throw(SQLerror);
 void addZusatzEntry(const AufEintragBase &AEB,const mengen_t &menge) throw(SQLerror);
 void addZusatzEntry_db(const AufEintragBase &AEB,const mengen_t &menge) throw(SQLerror);
 void updateZusatzEntry(const AufEintragBase &Z,const AuftragBase::mengen_t &menge) throw(SQLerror);
 void ZusaetzeLaden();
 friend FetchIStream& operator>>(FetchIStream& is,LieferscheinEntry &aeb);
 void showZusatzInfos() const;
};

#endif
