/* $Id: ArtikelImLager.cc,v 1.1 2002/11/22 15:58:20 christof Exp $ */
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

#include "ArtikelImLager.h"
#include <Misc/Trace.h>
#include <Auftrag/selFullAufEntry.h>
#include <Misc/relops.h>

ArtikelImLager::ArtikelImLager(const cH_ppsInstanz &instanz,const ArtikelBase &artikel,
   const Petig::Datum &datum) throw(SQLerror)
: menge_dispo_auftraege(0),menge_plan_auftraege(0)
{
  ManuProC::Trace _t(ManuProC::Tracer::Auftrag, __FUNCTION__,
     "Instanz=",instanz,"Artikel=",artikel);

  SelectedFullAufList auftraglist=SelectedFullAufList(SQLFullAuftragSelector::
     sel_Kunde_Artikel(instanz->Id(),Kunde::eigene_id,artikel));
  for (SelectedFullAufList::const_iterator i=auftraglist.begin();i!=auftraglist.end();++i)
   {
     // Nur wenn die freie Menge VOR dem Liefertermin frei wird AE verwenden
     
     if     (i->Id()==AuftragBase::none_id) ;
     else if(i->Id()==AuftragBase::ungeplante_id) ;
     else if(i->Id()==AuftragBase::dispo_auftrag_id)
      {
        assert(i->getRestStk()==i->getStueck());
        if(i->getLieferdatum() > datum) continue;
        menge_dispo_auftraege+=i->getRestStk();
        V_dispo_auftraege.push_back(*i);
      }
     else // AuftragBase::handplan_auftrag_id || plan_auftrag_id
      {
        if(i->getLieferdatum() <= datum) continue; 
        menge_plan_auftraege+=i->getRestStk();
        V_plan_auftraege.push_back(*i);
      }
   }
 assert(menge_dispo_auftraege>=0);
 assert(menge_plan_auftraege>=0);
}

