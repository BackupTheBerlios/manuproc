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

#ifndef _PRODLAGER_HH_
#define _PRODLAGER_HH_
#include <Aux/Datum.h>
#include <Auftrag/AufEintrag.h>
#include <Aux/ppsInstanz.h>

class Lager_Vormerkungen : AufEintrag 
{
   public:
      static const int LagerAuftragsId = 1;
   private:

      AuftragBase::mengen_t artikel_auf_lager(const ArtikelBase& artikel);
//      int Lieferzeit_in_Tagen(); 
   public:
      Lager_Vormerkungen(const AufEintrag&);


      // Das macht ein Auftrag:
      void vormerken_oder_bestellen();
      // Das macht das einlagern:
      void artikel_vormerken(AuftragBase::mengen_t menge);
      // Das macht das herausholen:
//      void artikel_ausliefern(AuftragBase::mengen_t menge);

      static void freigegeben_menge_neu_verplanen(cH_ppsInstanz instanz,const ArtikelBase& artikel,AuftragBase::mengen_t menge);

};

#endif
