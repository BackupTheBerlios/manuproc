// $Id: AuftragBase.cc,v 1.3 2002/10/04 08:23:20 thoma Exp $
/*  pps: ManuProC's production planning system
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

#include <Auftrag/AuftragBase.h>
#include<stdio.h>
#include <Aux/Transaction.h>
#include<Aux/Changejournal.h>
#include <Auftrag/auftrag_status.h>
#include "AufEintrag.h"
#include <Artikel/ArtikelStamm.h>
#include <Auftrag/AufEintragZu.h>
#include <Lager/Lager_Vormerkungen.h>

void AuftragBase::InstanzAuftraegeAnlegen(const ArtikelBase& artikel,
      const int altZnr,const Petig::Datum& lieferdatum,
      const AufStatVal status, int uid,const mengen_t menge) const
{
   ArtikelBaum AB(artikel);
//cout << cH_ArtikelBezeichnung(artikel)->Bezeichnung()<<'\n';
   for(ArtikelBaum::const_iterator i=AB.begin();i!=AB.end();++i)
    {
//cout << '\t'<<cH_ArtikelBezeichnung(i->rohartikel)->Bezeichnung()<<'\n';
      AuftragBase neuAuftrag(ArtikelStamm(i->rohartikel).BestellenBei(),0);
      Petig::Datum newdate=lieferdatum-Instanz()->ProduktionsDauer();
      neuAuftrag.tryUpdateEntry(double(i->menge)*menge,newdate,
                 i->rohartikel,status,uid,AufEintragBase(*this,altZnr));
    }
}

const std::string AuftragBase::getStatusStr(AufStatVal a)
{
 switch(a)
   {case (AufStatVal)UNCOMMITED : return "unbestätigt"; 
    case (AufStatVal)OPEN : return "offen"; 
    case (AufStatVal)CLOSED : return "fertig";
    case (AufStatVal)STORNO : return "storno";
    case (AufStatVal)NOSTAT : ;
   }
 return "-";
}

const int AuftragBase::PlanId_for(const cH_ppsInstanz &instanz)
{
  if(instanz->LagerInstanz()) return AuftragBase::plan_auftrag_id;
  return AuftragBase::handplan_auftrag_id;
}
