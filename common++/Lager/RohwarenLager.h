/* $Id: RohwarenLager.h,v 1.11 2006/02/08 17:09:45 christof Exp $ */
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

#ifndef _ROHWARENLAGER_HH_
#define _ROHWARENLAGER_HH_

#include "Lager.h"
#include <Artikel/Einheiten.h>
#include <Auftrag/AuftragBase.h>

class RohwarenLager : public LagerBase
{
   public:
      enum RL_LogTyp { RLNone, RLEntnehmen='E', RLEinlagern='L' };
      struct st_rohlager
      {     LagerPlatz position;
	    int kartons;
	    int kg_pro_karton;
            int reste;
            int reste_kg; 
            ArtikelBase artikel; 
            ManuProC::Datum datum;
            
           st_rohlager( LagerPlatz p,int k,int kpk,int r,int rkg, 
                        ArtikelBase a, ManuProC::Datum d)
            : position(p),kartons(k),kg_pro_karton(kpk),reste(r),
               reste_kg(rkg),artikel(a),datum(d) {}
           void setPosition(const LagerPlatz p) {position=p;}
           bool empty() const { return kartons==0 && reste==0; }
           std::string Menge_als_Text(Einheit const& e) const;
           AuftragBase::mengen_t Menge_fuer_Auftrag() const;
      };       

   private:

      void RL_Log(const st_rohlager& rohlager,RL_LogTyp typ,const std::string& misc="");
      void RL_Einlagern_real(st_rohlager& rohlager,bool geliefert);
      void auftragsentry_verwaltung(const RL_LogTyp typ,const ArtikelBase &artikel,const AuftragBase::mengen_t &menge) const;
      virtual std::vector<class LagerInhalt> LagerInhalt_(const ArtikelBase& artikel) const;

   public:
#if defined PETIG_EXTENSIONS && defined MANUPROC_DYNAMICENUMS_CREATED
      RohwarenLager() : LagerBase(ppsInstanzID::Rohwarenlager) {}
#else
      RohwarenLager() : LagerBase(ppsInstanz::default_id) {}
#endif

      st_rohlager RL_Inhalt(const LagerPlatz position) const;
      // force legt fest, ob ein bestehender Inhalt überschrieben weren soll
      // defautmäßig wird nur dann eingelagert, wenn die Lagerposition leer ist.
      bool RL_Einlagern(const LagerPlatz position,st_rohlager& rohlager,std::string &os,bool force,bool geliefert);
      bool RL_Entnahme(st_rohlager& rohlager,std::string &os,bool force,bool ist_leer,bool fuer_auftrag);
};

#endif
