// $Id: Lager.cc,v 1.24 2003/01/31 16:23:15 christof Exp $
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

#include "Lager.h"
#include <Misc/Transaction.h>
#include <algorithm>
#include <Auftrag/ppsInstanzProduziert.h>
#include <Misc/relops.h>
#include <Misc/Trace.h>


Lager::Lager(cH_ppsInstanz instanz)
: LagerBase(instanz)
{
#if defined PETIG_EXTENSIONS && defined MANUPROC_DYNAMICENUMS_CREATED
   if(instanz==ppsInstanzID::Rohwarenlager) 
     assert(!"Use class 'RohwarenLager()' instead of 'Lager(ppsInstanzID::Rohwarenlager)'\n");
   else if(instanz==ppsInstanzID::Bandlager)
     assert(!"Use class 'JumboLager()' instead of 'Lager(ppsInstanzID::Bandlager)'\n");
#endif        
}


void LagerBase::rein_ins_lager(const ArtikelBase &artikel,const AuftragBase::mengen_t &menge,const int uid) const
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,
     "Lager=",*this,"Artikel=",artikel,"Menge=",menge);
  assert(menge>=0);

  AuftragBase::menge_neu_verplanen(uid,*this,artikel,menge);
}

void LagerBase::raus_aus_lager(const ArtikelBase &artikel,const AuftragBase::mengen_t &menge,const int uid) const 
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,
     "Artikel=",artikel,"Menge=",menge);
  assert(menge>=0);

    assert((*this)->ProduziertSelbst());

     ManuProC::st_produziert sp(artikel,menge,uid);
     (*this)->Lager_abschreiben(sp);
}


std::vector<class LagerInhalt> LagerBase::LagerInhalt()  const
{
  return LagerInhalt_(ArtikelBase());
} 
  
class LagerInhalt LagerBase::LagerInhalt(const ArtikelBase& artikel) const
{
  std::vector<class LagerInhalt> L=LagerInhalt_(artikel);
  if(L.empty()) return class LagerInhalt(artikel,0,0,0,0);
  return *(L.begin());
}
        
void LagerBase::LagerInhaltSum(std::vector<class LagerInhalt>& LI) 
{
  std::sort(LI.begin(),LI.end());  
  std::vector<class LagerInhalt>::iterator i,j,k;
  for(i=LI.begin();i!=LI.end();)
   {
     j=i; ++j;
     if(*i!=*j) {++i; continue;}
     k=j;
     for(;k!=LI.end() && *i==*k ;++k)  *i+=*k ;
     i=LI.erase(j,k);     
   }
}

        