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

#ifndef _JUMBOLAGER_HH_
#define _JUMBOLAGER_HH_

#include "Lager.h"
#include "JumboRolle.h"


class JumboLager : public Lager
{
  public:
      enum Jumbo_LogTyp {None, Umraeumen='1',SchonDraussen='2',Inventur='i', Inventur_Ohne_Bestand='j',
            Einlagern='A', Auslagern='B', Zurueck='C', InventurEnde='E',
            InventurPlus='I', InventurMinus='J',
            Einlagern_L='L', Rest_L='Q', Rest='R'  };

  private:
      static const float reste_faktor = 0.3;
/*
      struct st_menge_sortiert{LagerPlatz position; int meter; int rollen;     
                               int restmeter;int restrollen;int gesamtmeter;   
             st_menge_sortiert(LagerPlatz p, int m, int r,int rm,int rr,int gm)
               :position(p),meter(m),rollen(r),
                  restmeter(rm),restrollen(rr),gesamtmeter(gm) {}
            };
      struct st_wo_ist_wieviel{LagerPlatz position;int meter;bool rest;int code;
             st_wo_ist_wieviel(LagerPlatz p,int m,bool r, int c)
              :position(p),meter(m),rest(r),code(c){}};
      struct st_wo_ist_wieviel_rest{int volleRollen;
               int volleRollenMeter; int restRollen; int restRollenMeter;
               int GesamtGewichtetMeter;
             st_wo_ist_wieviel_rest()
               :volleRollen(0),volleRollenMeter(0),restRollen(0),
                  restRollenMeter(0),GesamtGewichtetMeter(0) {}
               void add_volleRolle() {++volleRollen;}
               void add_volleRolleMeter(int i) {volleRollenMeter+=i;}
               void add_restRolle() {++volleRollen;}
               void add_restRolleMeter(int i) {restRollenMeter+=i;}  
               void add_gesamtMeter(int i) {GesamtGewichtetMeter+=i;}
            };
*/

      void Jumbo_Log(const JumboRolle& jumbo,Jumbo_LogTyp typ,const string& user,const Zeitpunkt_new *zeit);
//      std::vector<st_wo_ist_wieviel> getPosition(ArtikelBase artikel);
//      std::map<LagerPlatz,st_wo_ist_wieviel_rest>  getPositionSorted(ArtikelBase artikel);

/*
      class Sort{
           public:
             Sort(){}
             bool operator()(const st_menge_sortiert& x,const st_menge_sortiert& y) const
               { return (x.restrollen<y.restrollen ||
                        (x.restrollen==y.restrollen && x.rollen<y.rollen));}
         };
*/
      virtual std::vector<class LagerInhalt> LagerInhalt_(const ArtikelBase& artikel=ArtikelBase()) const;

  public:
#if defined PETIG_EXTENSIONS && defined MANUPROC_DYNAMICENUMS_CREATED
      JumboLager() : Lager(ppsInstanzID::Bandlager) {}
#endif

      void Jumbo_Einlagern(const LagerPlatz position,JumboRolle& jumbo,Jumbo_LogTyp typ,const string& user,const Zeitpunkt_new *zeit=0);
      void Jumbo_Entnahme(JumboRolle& jumbo,Jumbo_LogTyp typ,const string& user,const Zeitpunkt_new *zeit=0);
      //alle Codes für eine Lagerposition
//      std::vector<int> Jumbo_Inhalt(const LagerPlatz position) const;
//      std::list<JumboLager::st_menge_sortiert>  getMengeSorted(ArtikelBase artikel);
      //Lagerinhalt
//      virtual std::vector<class LagerInhalt> LagerInhalt();
//      class LagerInhalt LagerInhalt(const ArtikelBase& artikel);

};

#endif
