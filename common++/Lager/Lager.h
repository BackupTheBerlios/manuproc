/* $Id: Lager.h,v 1.21 2003/07/08 07:24:22 christof Exp $ */
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
#include <Artikel/ArtikelBase.h>
class ppsInstanzReparatur;
#include "LagerPlatz.hh"
#include <Auftrag/AuftragBase.h>
#include <vector>
#include <Misc/Handles.h>
#include <Auftrag/selFullAufEntry.h>
#include <Auftrag/auftrag_enums.h>
class FetchIStream;

class LagerInhalt
{	friend FetchIStream &operator>>(FetchIStream &is, LagerInhalt &li);
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
      int Stueck() const {return stueck;}
      int RestStueck() const {return reststueck;}
      int GesamtStueck() const {return stueck+reststueck;}
      int Menge() const {return menge;}
      int RestMenge() const {return restmenge;}
      int GesamtMenge() const {return menge+restmenge;}
      
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


class LagerBase : public cH_ppsInstanz
{
   public:
      /// Datum f�r freie Lagermengen (Auftr�ge)
      static ManuProC::Datum Lagerdatum() {return ManuProC::Datum(ManuProC::Datum(1,1,1970));}
   private:
      virtual std::vector<class LagerInhalt> LagerInhalt_(const ArtikelBase& artikel)
        const {assert(!"Nicht implementiert f�r Lager ohne eigene Tabelle");abort();} 

     // die folgenden Methoden m�ssen einlagern k�nnen
     friend class ppsInstanzReparatur;
     friend void AufEintrag::ProduziertNG(unsigned, fixedpoint<0>, const AufEintragBase&, const AufEintragBase&);

   protected:
      LagerBase(cH_ppsInstanz  instanz) : cH_ppsInstanz(instanz) {}

      virtual ~LagerBase(){}

   public:
      std::vector<class LagerInhalt> LagerInhalt() const ;
      // Fa�t gleiche Artikel des Vektors zusammenen:
      static void LagerInhaltSum(std::vector<class LagerInhalt>& LI);
      class LagerInhalt LagerInhalt(const ArtikelBase& artikel) const ;

   protected:
     // produziert bedeutet, dass der Artikel hergestellt wurde 
     // und nicht irgendwo "gefunden" (z.B. Inventur)
     void rein_ins_lager(const ArtikelBase &artikel,const AuftragBase::mengen_t &menge,const int uid,bool produziert) const;
     void wiedereinlagern(const ArtikelBase &artikel,const AuftragBase::mengen_t &menge,const int uid) const;
     // fuer_auftrag bedeutet, dass der Artikel f�r einen Auftrag verwendet wurde
     // und nicht verschwunden ist (Entnahme au�er der Reihe/Inventur)
     void raus_aus_lager(const ArtikelBase &artikel,AuftragBase::mengen_t menge,const int uid,bool fuer_auftrag) const;
};

class Lager : public LagerBase
{
  public:  
      Lager(cH_ppsInstanz  instanz) ;
// make these routines public      
     void rein_ins_lager(const ArtikelBase &artikel,const AuftragBase::mengen_t &menge,unsigned uid,bool produziert) const
     {  LagerBase::rein_ins_lager(artikel,menge,uid,produziert); }
     void wiedereinlagern(const ArtikelBase &artikel,const AuftragBase::mengen_t &menge,unsigned uid) const
     {  LagerBase::wiedereinlagern(artikel,menge,uid); }
     void raus_aus_lager(const ArtikelBase &artikel,AuftragBase::mengen_t menge,unsigned uid,bool fuer_auftrag) const
     {  LagerBase::raus_aus_lager(artikel,menge,uid,fuer_auftrag); }
};
#endif
