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
                   const Petig::Datum &datum=Lager::Lagerdatum());
//      int Lieferzeit_in_Tagen(); 
   public:
      Lager_Vormerkungen(const AufEintrag&);

      // Das macht ein Auftrag:
      void vormerken_oder_bestellen(int uid);

      // Das macht das einlagern:
      static void freigegeben_menge_neu_verplanen(cH_ppsInstanz instanz,const ArtikelBase& artikel,AuftragBase::mengen_t menge,int uid);
 private:
      void reduziere_ungeplant(int uid,AuftragBase::mengen_t menge);
      void artikel_vormerken(AuftragBase::mengen_t menge,const ArtikelBase &artikel,int uid,
         std::vector<pair<AufEintragBase,AuftragBase::mengen_t> > dispo_auftrag);
      void artikel_schnappen(AuftragBase::mengen_t menge,const ArtikelBase &artikel,int uid,
         std::vector<pair<AufEintragBase,AuftragBase::mengen_t> > dispo_auftrag);

};

#endif
