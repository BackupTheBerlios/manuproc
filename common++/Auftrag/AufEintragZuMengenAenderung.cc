// $Id: AufEintragZuMengenAenderung.cc,v 1.20 2003/09/02 12:10:52 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2003 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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

#include <Auftrag/AufEintragZu.h>  
#include <Auftrag/AufEintrag.h>  
#include <Auftrag/AufEintragZuMengenAenderung.h>
#include <Misc/TraceNV.h>
#include <Misc/relops.h>

// ElternAEB erhält die Menge vom 2er
// dies scheint den 1er im Lager zu erhöhen
void AufEintragZuMengenAenderung::freie_dispomenge_verwenden(
         const AufEintrag &AE2er,AuftragBase::mengen_t menge,const AufEintragBase &ElternAEB) throw(SQLerror)
{
  AufEintragZu::list_t L=AufEintragZu(AE2er).get_Referenz_list_geplant();
  for(AufEintragZu::list_t::reverse_iterator i=L.rbegin();i!=L.rend();++i)
   {
    AuftragBase::mengen_t M=AuftragBase::min(i->Menge,menge);
    AufEintragZu(AE2er).setMengeDiff__(i->AEB,-M);
    AufEintragZu(ElternAEB).Neu(i->AEB,M);
    menge-=M;
    if(!menge) break;
   }
}
