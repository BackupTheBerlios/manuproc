/* $Id: Lager_Vormerkungen.h,v 1.18 2002/07/11 07:07:42 malte Exp $ */
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

#ifndef _PRODLAGER_HH_
#define _PRODLAGER_HH_
#include <Aux/Datum.h>
#include <Auftrag/AufEintrag.h>
#include <Instanzen/ppsInstanz.h>
#include <Lager/Lager.h>

class Lager_Vormerkungen : AufEintrag 
{
   private:

      AuftragBase::mengen_t artikel_auf_lager(std::vector<pair<AufEintragBase,AuftragBase::mengen_t> > &dispo_auftrag,bool freie_menge=true);
      static AuftragBase::mengen_t artikel_auf_lager(const ArtikelBase &artikel,
                                                     cH_ppsInstanz instanz,
                   std::vector<pair<AufEintragBase,AuftragBase::mengen_t> >&dispo_auftrag,
                   const Petig::Datum &datum,int auftragsid);
//      int Lieferzeit_in_Tagen(); 
        void reduce_old_plan_auftrag(int uid,AufEintragBase dispo_aeb,mengen_t menge);
 
        void move_menge_from_dispo_to_plan(int uid,AufEintragBase dispo_aeb,mengen_t menge);

   public:
      Lager_Vormerkungen(const AufEintrag&);

 
        void move_menge_from_dispo_to_plan2(int uid,AufEintragBase dispo_aeb,mengen_t menge);
      // verfügbare Menge für einen Artikel bestimmen
      static AuftragBase::mengen_t artikel_auf_lager(const ArtikelBase artikel,cH_ppsInstanz instanz,const Petig::Datum &datum,int auftragsid)
         { std::vector<pair<AufEintragBase,AuftragBase::mengen_t> > d;
           return artikel_auf_lager(artikel,instanz,d,datum,auftragsid);}

      // Das macht ein Auftrag:
      // Rückgabewert: Lagermenge
      void vormerken_oder_bestellen(int uid,AuftragBase::mengen_t vormerkmenge,AuftragBase::mengen_t bestellmenge);

      // Das macht das einlagern:
      static void freigegeben_menge_neu_verplanen(cH_ppsInstanz instanz,const ArtikelBase& artikel,AuftragBase::mengen_t menge,int uid,bool produziert);
 private:
//      void reduziere_ungeplant(int uid,AuftragBase::mengen_t menge);
      // vorgemerkt wird ein 2er Artikel
      // geschnappt wird ein 1er Artikel
      void artikel_vormerken_oder_schnappen(bool schnappen,AuftragBase::mengen_t menge,const ArtikelBase &artikel,int uid,
         std::vector<pair<AufEintragBase,AuftragBase::mengen_t> > dispo_auftrag,bool reduce_old);

};

#endif
