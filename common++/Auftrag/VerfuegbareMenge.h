/* $Id: VerfuegbareMenge.h,v 1.5 2003/09/02 12:10:52 christof Exp $ */
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

#ifndef ARTIKELIMLAGER
#define ARTIKELIMLAGER

#include <Artikel/ArtikelBase.h>
#include <Auftrag/AuftragBase.h>
#include <Auftrag/AufEintrag.h>

class VerfuegbareMenge : public ArtikelBase
{
      AuftragBase::mengen_t menge_dispo_auftraege;
      AuftragBase::mengen_t menge_plan_auftraege;

      std::vector<AufEintrag> V_dispo_auftraege;
      std::vector<AufEintrag> V_plan_auftraege;
      
      ManuProC::Datum datum;
      cH_ppsInstanz instanz;

      AuftragBase::mengen_t reduce_in_dispo_or_plan(const bool dispo,
      		AuftragBase::mengen_t menge,
      		const AufEintragBase &ElternAEB);

   public:
      VerfuegbareMenge(const cH_ppsInstanz &instanz,const ArtikelBase &artikel,const ManuProC::Datum &datum) throw(SQLerror);

      AuftragBase::mengen_t getMengeDispo() const {return menge_dispo_auftraege;}
      AuftragBase::mengen_t getMengePlan() const {return menge_plan_auftraege;}

      std::vector<AufEintrag> getDispoAuftraege() const {return V_dispo_auftraege;}
      std::vector<AufEintrag> getPlanAuftraege() const {return V_plan_auftraege;}

	// wird von AE::ArtikelInternNachbestellen verwendet
      AuftragBase::mengen_t reduce_in_dispo(AuftragBase::mengen_t menge,const AufEintragBase &ElternAEB)
         { return reduce_in_dispo_or_plan(true,menge,ElternAEB);}
      AuftragBase::mengen_t reduce_in_plan(AuftragBase::mengen_t menge,const AufEintragBase &ElternAEB)
         { return reduce_in_dispo_or_plan(false,menge,ElternAEB);}
};

#endif
