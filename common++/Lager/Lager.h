/* $Id: Lager.h,v 1.32 2005/10/12 08:59:39 christof Exp $ */
/*  pps: ManuProC's production planning system
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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

#ifndef _LAGER_HH_
#define _LAGER_HH_
#include <vector>
//#include <Misc/Handles.h>
#include <Artikel/ArtikelBase.h>
#include <Auftrag/AufEintragBase.h>
#include <Lieferschein/LieferscheinEntryBase.h>
#include <Lager/LagerPlatz.hh>
#include <Misc/Datum.h>
//#include <Auftrag/selFullAufEntry.h>
//#include <Auftrag/auftrag_enums.h>
class ppsInstanzReparatur;
class Query_Row;

class LagerInhalt
{	friend Query_Row &operator>>(Query_Row &is, LagerInhalt &li);
   private:
      ArtikelBase artikel;
      int stueck,reststueck;
      int menge, restmenge;

   public:
          
      LagerInhalt(ArtikelBase a=ArtikelBase())
      		: artikel(a),stueck(),reststueck(),menge(), restmenge() {}
      LagerInhalt(ArtikelBase a, int s, int rs, int m, int rm)
               : artikel(a),stueck(s),reststueck(rs),menge(m),restmenge(rm){}

      ArtikelBase Artikel() const {return artikel;}

      // Bandlager: Stueck=Rollen , Menge (m) 
      // Rohwarenlager: Stueck=Kartons, Menge(kg)
      __deprecated      int Stueck() const {return stueck;}
      __deprecated int RestStueck() const {return reststueck;}
      __deprecated int GesamtStueck() const {return stueck+reststueck;}
      int Menge() const {return menge;}
      __deprecated      int RestMenge() const {return restmenge;}
      __deprecated      int GesamtMenge() const {return menge+restmenge;}
      
      bool operator==(const LagerInhalt& b) const {return Artikel()==b.Artikel();}
      bool operator<(const LagerInhalt& b) const  {return Artikel()<b.Artikel();}
      LagerInhalt operator+=(const LagerInhalt& b)
         {
           stueck+=b.stueck;
           reststueck+=b.reststueck;
           menge+=b.menge;
           restmenge+=b.restmenge;
           return *this;
         }
};

struct ProductionContext
{  // Einlagern: Auftrag nach unten der erfllt wird
   // Auslagern: Auftrag fr den es ausgelagert wird
   AufEintragBase aeb;

   // Betroffener Lagerauftrag (optional!)
   AufEintragBase lager_aeb;
   
   // zum Loggen: Lieferscheinzeile fr diese Bewegung
   LieferscheinEntryBase leb; // Auslagern
   
   mutable bool fuer_auftrag;
   
   ProductionContext():fuer_auftrag(false) {}
   ProductionContext(const AufEintragBase &a,const LieferscheinEntryBase &l,
   		const AufEintragBase &la=AufEintragBase())
   	: aeb(a), lager_aeb(la), leb(l), fuer_auftrag(false) {}
   ProductionContext(const LieferscheinEntryBase &l)
   	: leb(l), fuer_auftrag(false) {}
};
std::ostream &operator<<(std::ostream &o,const ProductionContext &pc);

class LagerBase : public cH_ppsInstanz
{
   public:
      /// Datum fr freie Lagermengen (Auftr�e)
      static ManuProC::Datum Lagerdatum() {return ManuProC::Datum(ManuProC::Datum(1,1,1970));}
   private:
      virtual std::vector<class LagerInhalt> LagerInhalt_(const ArtikelBase& artikel)
        const {assert(!"Nicht implementiert fr Lager ohne eigene Tabelle");abort();} 

     // die folgenden Methoden mssen einlagern k�nen
     friend class ppsInstanzReparatur;
//     friend void AufEintrag::ProduziertNG(unsigned, fixedpoint<0>, const AufEintragBase&, const AufEintragBase&);

   protected:
      LagerBase(cH_ppsInstanz  instanz) : cH_ppsInstanz(instanz) {}

      virtual ~LagerBase(){}

   public:
      std::vector<class LagerInhalt> LagerInhalt() const ;
      // Fa� gleiche Artikel des Vektors zusammenen:
      static void LagerInhaltSum(std::vector<class LagerInhalt>& LI);
      class LagerInhalt LagerInhalt(const ArtikelBase& artikel) const ;

   protected:
     // produziert bedeutet, dass der Artikel hergestellt wurde 
     // und nicht irgendwo "gefunden" (z.B. Inventur)
     void rein_ins_lager(const ArtikelBase &artikel,
     	const AuftragBase::mengen_t &menge,bool produziert,
     	const ProductionContext &ctx=ProductionContext()) const;
     
     // entspricht raus_aus_lager mit negativer Menge
     __deprecated void wiedereinlagern(const ArtikelBase &artikel,
     	const AuftragBase::mengen_t &menge) const
     {  raus_aus_lager(artikel,-menge,false); }
     
     // fuer_auftrag bedeutet, dass der Artikel fr einen Auftrag verwendet wurde
     // und nicht verschwunden ist (Entnahme au�r der Reihe/Inventur)
     //
     // negative Menge bedeutet: Lieferung rckg�gig gemacht
     // = wiedereinlagern
     void raus_aus_lager(const ArtikelBase &artikel,
     	AuftragBase::mengen_t menge,bool fuer_auftrag,
     	const ProductionContext &ctx=ProductionContext()) const;
};

class Lager : public LagerBase
{
  public:  
      Lager(cH_ppsInstanz  instanz) ;
// make these routines public      
     void rein_ins_lager(const ArtikelBase &artikel,
     	const AuftragBase::mengen_t &menge,bool produziert,
     	const ProductionContext &ctx=ProductionContext()) const;
     void raus_aus_lager(const ArtikelBase &artikel,
     	AuftragBase::mengen_t menge,bool fuer_auftrag,
     	const ProductionContext &ctx=ProductionContext()) const;

     __deprecated void wiedereinlagern(const ArtikelBase &artikel,
     			const AuftragBase::mengen_t &menge) const;
};
#endif
