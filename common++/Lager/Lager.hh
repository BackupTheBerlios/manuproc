// $Id: identify,v 1.6 1998/06/04 07:38:44 cvs Exp $
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
#include <Aux/Datum.h>
#include <Aux/Zeitpunkt_new.h>
#include <Artikel/ArtikelBase.h>
//#include <Auftrag/AufEintrag.h>
#include <Aux/ppsInstanz.h>
#include "LagerPlatz.hh"
#include <Auftrag/AuftragBase.h>
#include <vector>
#include <list>
#include "JumboRolle.h"

// Demnächst mal zwei Klassen JumboLager und GarnLager von der Klasse 'Lager' 
// ableiten und die entsprechenden Funktionen da rein. 17.1.2002 MAT

class Lager {
      ppsInstanz::ppsInstId instanz;
   public:
      enum RL_LogTyp { RLNone, RLEntnehmen='E', RLEinlagern='L' };
      enum Jumbo_LogTyp {None, Umraeumen='1',SchonDraussen='2',Inventur='i', Inventur_Ohne_Bestand='j',
            Einlagern='A', Auslagern='B', Zurueck='C', InventurEnde='E',
            InventurPlus='I', InventurMinus='J',
            Einlagern_L='L', Rest_L='Q', Rest='R'  };

      struct st_rohlager{LagerPlatz position;int kartons;int kg_pro_karton;
            int reste;int reste_kg; ArtikelBase artikel; ManuProC::Datum datum;
           st_rohlager( LagerPlatz p,int k,int kpk,int r,int rkg, 
                        ArtikelBase a, ManuProC::Datum d)
            : position(p),kartons(k),kg_pro_karton(kpk),reste(r),
               reste_kg(rkg),artikel(a),datum(d) {}
             void setPosition(const LagerPlatz p) {position=p;}
             bool empty() 
               {if (kartons==0 && reste==0) return true; 
                else return false;} };       

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
      struct st_menge_sortiert{LagerPlatz position; int meter; int rollen;
                               int restmeter;int restrollen;int gesamtmeter;
             st_menge_sortiert(LagerPlatz p, int m, int r,int rm,int rr,int gm)
               :position(p),meter(m),rollen(r),
                  restmeter(rm),restrollen(rr),gesamtmeter(gm) {}
            };

   private:
       
      void check_lager_is_valid();

      void RL_Log(const st_rohlager& rohlager,RL_LogTyp typ,const std::string& misc="");
      void Jumbo_Log(const JumboRolle& jumbo,Jumbo_LogTyp typ,const string& user,const Zeitpunkt_new *zeit);
      std::vector<st_wo_ist_wieviel> getPosition(ArtikelBase artikel);
      std::map<LagerPlatz,st_wo_ist_wieviel_rest>  getPositionSorted(ArtikelBase artikel);


      class Sort{
           public:
             Sort(){}
             bool operator()(const st_menge_sortiert& x,const st_menge_sortiert& y) const
               { return (x.restrollen<y.restrollen ||
                        (x.restrollen==y.restrollen && x.rollen<y.rollen));}
         };

      void bewegung(bool raus,ArtikelBase artikel,AuftragBase::mengen_t menge);

   public:
      Lager(ppsInstanz::ppsInstId _instanz);

      // Rohwarenlager:
      // force legt fest, ob ein bestehender Inhalt überschrieben weren soll
      // defautmäßig wird nur dann eingelagert, wenn die Lagerposition leer ist.
      bool RL_Einlagern(const LagerPlatz position,st_rohlager& rohlager,std::string &os,bool force=false);
      bool RL_Entnahme(st_rohlager& rohlager,std::string &os,bool force=false,bool ist_leer=false);
      st_rohlager RL_Inhalt(const LagerPlatz position) const;

      // JumboRollenlager:
      void Jumbo_Einlagern(const LagerPlatz position,JumboRolle& jumbo,Jumbo_LogTyp typ,const string& user,const Zeitpunkt_new *zeit=0);
      void Jumbo_Entnahme(JumboRolle& jumbo,Jumbo_LogTyp typ,const string& user,const Zeitpunkt_new *zeit=0);
         //alle Codes für eine Lagerposition
      std::vector<int> Jumbo_Inhalt(const LagerPlatz position) const; 
      std::list<Lager::st_menge_sortiert>  getMengeSorted(ArtikelBase artikel);

      void rein_ins_lager(ArtikelBase artikel,AuftragBase::mengen_t menge);
      void raus_aus_lager(ArtikelBase artikel,AuftragBase::mengen_t menge);

};

#endif

