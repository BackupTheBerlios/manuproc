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
//#include <Auftrag/AufEintragBase.h>
#include <Aux/ppsInstanz.h>
#include "LagerPlatz.hh"
#include <Auftrag/AuftragBase.h>

class Lager {
      ppsInstanz::ppsInstId instanz;
   public:
      struct st_rohlager{LagerPlatz position;int kartons;int kg_pro_karton;
            int reste;int reste_kg; ArtikelBase artikel; Petig::Datum datum;
           st_rohlager( LagerPlatz p,int k,int kpk,int r,int rkg, 
                        ArtikelBase a, Petig::Datum d)
            : position(p),kartons(k),kg_pro_karton(kpk),reste(r),
               reste_kg(rkg),artikel(a),datum(d) {}
             bool empty() 
               {if (kartons==0 && reste==0) return true; 
                else return false;} };       

      struct st_bandlager{LagerPlatz position;int code;
            int soll_meter; bool rest; ArtikelBase artikel; 
            Zeitpunkt_new *zeit; int status; char action;
            std::string username;
           st_bandlager( LagerPlatz p,int c,int sm,bool r,
                        ArtikelBase a, Zeitpunkt_new *z,int s,char ac)
            : position(p),code(c),soll_meter(sm),rest(r),
               artikel(a),zeit(z),status(s),action(ac) {}
//             bool empty() 
//               {if (kartons==0 && reste==0) return true; 
//                else return false;}
             };       

   private:
       
      void check_lager_is_valid();
      enum RL_LogTyp { Entnehmen='E', Einlagern='L' };
      enum Jumbo_LogTyp { JEntnehmen='E', JEinlagern='L' };
      void RL_Log(const st_rohlager& rohlager,RL_LogTyp typ,const std::string& misc="");
      void abschreiben(RL_LogTyp typ,ArtikelBase artikel,AuftragBase::mengen_t menge);

   public:
      Lager(ppsInstanz::ppsInstId _instanz);

      // Rohwarenlager:
      // force legt fest, ob ein bestehender Inhalt überschrieben weren soll
      // defautmäßig wird nur dann eingelagert, wenn die Lagerposition leer ist.
      bool RL_Einlagern(st_rohlager& rohlager,std::string &os,bool force=false);
      bool RL_Entnahme(st_rohlager& rohlager,std::string &os,bool force=false,bool ist_leer=false);
      st_rohlager RL_Inhalt(const LagerPlatz position);

      // JumboRollenlager:
      bool Jumbo_Einlagern(st_bandlager bandlager,Jumbo_LogTyp typ);
      bool Jumbo_Entnahme(st_bandlager bandlager,Jumbo_LogTyp typ);

};

#endif

