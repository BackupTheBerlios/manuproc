// $Id: Lager.cc,v 1.20 2002/12/17 22:40:14 jacek Exp $
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
#include <Aux/Transaction.h>
#include <algorithm>
#include <Instanzen/ppsInstanzProduziert.h>
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

     Transaction tr;
     AuftragBase::dispo_auftrag_aendern(uid,*this,artikel,menge);
     AuftragBase::menge_neu_verplanen(uid,*this,artikel,menge,ManuProC::Auftrag::r_Produziert);     

     ManuProC::st_produziert sp(artikel,menge,uid);
     
     if(ppsInstanz::getProduktionsInstanz(artikel)->PlanungsInstanz())
        (*this)->Produziert(sp,ManuProC::Auftrag::r_Produziert);
     else if(!ppsInstanz::getProduktionsInstanz(artikel)->ProduziertSelbst())
           ppsInstanz::getProduktionsInstanz(artikel)->Produziert(sp);


     tr.commit();

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


/*
void LagerBase::menge_neu_verplanen(int uid,
              const ArtikelBase& artikel,AuftragBase::mengen_t menge,
              const ManuProC::Auftrag::Action reason) throw(SQLerror)
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,"Instanz=",*this,
   "Artikel=",artikel,"Menge=",menge,
     "Reason=",reason);

  assert(menge>=0);
  if(menge==AuftragBase::mengen_t(0)) return;

  SQLFullAuftragSelector sel(SQLFullAuftragSelector::
      sel_Artikel_Planung_id((*this)->Id(),
      Kunde::eigene_id,artikel,AuftragBase::ungeplante_id));
  SelectedFullAufList auftraglist=SelectedFullAufList(sel);
  
  for (SelectedFullAufList::iterator i=auftraglist.begin();i!=auftraglist.end();++i)
   {
     if(i->getRestStk()==AuftragBase::mengen_t(0)) continue;
     AuftragBase::mengen_t M=i->getRestStk();
     if(M>=menge) M=menge;

     ArtikelImLager AIL(*this,i->Artikel(),i->getLieferdatum());
     i->artikel_vormerken_oder_schnappen(false,M,i->Artikel(),uid,reason,AIL.getDispoAuftraege());
     if(reason==ManuProC::Auftrag::r_Anlegen || reason==ManuProC::Auftrag::r_Planen||
        reason==ManuProC::Auftrag::r_Closed)    
            i->updateStkDiffInstanz__(uid,-M,reason);
     menge-=M;
     if(menge==AuftragBase::mengen_t(0)) return;
   }
}
*/

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

        