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
//#include <Aux/Datum.h>
//#include <Aux/Zeitpunkt_new.h>
#include <Artikel/ArtikelBase.h>
//#include <Auftrag/AufEintrag.h>
#include <Aux/ppsInstanz.h>
#include "LagerPlatz.hh"
#include <Auftrag/AuftragBase.h>
#include <vector>
//#include <list>
#include <Aux/Handles.h>

class LagerInhalt
{
   private:
      ArtikelBase artikel;
      int stueck,reststueck;
      int menge, restmenge;

   public:
      LagerInhalt(ArtikelBase a) 
               : artikel(a),stueck(0),reststueck(0),menge(0),restmenge(0){}
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


class Lager : public HandleContent 
{
   private:
      ppsInstanz::ID instanz;
      void bewegung(bool raus,ArtikelBase artikel,AuftragBase::mengen_t menge);
      virtual std::vector<class LagerInhalt> LagerInhalt_(const ArtikelBase& artikel)const=0 ;

   protected:
      Lager(ppsInstanz::ID _instanz);

   public:
      std::vector<class LagerInhalt> LagerInhalt() const ;
      // Faﬂt gleiche Artikel des Vektors zusammenen:
      static void LagerInhaltSum(std::vector<class LagerInhalt>& LI);
      class LagerInhalt LagerInhalt(const ArtikelBase& artikel) const ;

      void rein_ins_lager(ArtikelBase artikel,AuftragBase::mengen_t menge);
      void raus_aus_lager(ArtikelBase artikel,AuftragBase::mengen_t menge);

};

class H_Lager : public Handle<Lager>
{
  private:
     H_Lager(Lager *l) : Handle<Lager>(l){} 
  public:
     H_Lager(const ArtikelBase& artikel) ;
     H_Lager(const cH_ppsInstanz& instanz) ;
};


#endif

