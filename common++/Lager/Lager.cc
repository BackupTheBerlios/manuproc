// $Id: Lager.cc,v 1.17 2002/11/22 15:31:05 christof Exp $
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
#ifdef PETIG_EXTENSIONS
#include "JumboLager.h"
#include "RohwarenLager.h"
#endif
#include <Auftrag/AufEintrag.h>
#include <Auftrag/Auftrag.h>
#include <Auftrag/AufEintragZu.h>
#include <Auftrag/selFullAufEntry.h>
#include <Instanzen/ppsInstanz.h>
#include <Aux/Transaction.h>
#include <Artikel/ArtikelStamm.h>
#include <Artikel/ArtikelImLager.h>
#include <algorithm>
#include <unistd.h>
#include <Aux/AdminProblems.h>
//#include <Instanzen/Produziert.h>
#include <Instanzen/ppsInstanzProduziert.h>
#include <Misc/relops.h>
#include <Misc/Trace.h>

void Lager::rein_ins_lager(ArtikelBase artikel,AuftragBase::mengen_t menge,int uid)
{
  ManuProC::Trace _t(ManuProC::Tracer::Auftrag, __FUNCTION__,
     "Lager=",*this,"Artikel=",artikel,"Menge=",menge);
  assert(menge>=0);
  try{
     Transaction tr;
     dispo_auftrag_aendern(artikel,menge);
     menge_neu_verplanen(uid,artikel,menge,ManuProC::Auftrag::r_Produziert);     

     ManuProC::st_produziert sp(artikel,menge,uid);
     
     if(ppsInstanz::getProduktionsInstanz(artikel)->PlanungsInstanz())
        (*this)->Produziert(sp,ManuProC::Auftrag::r_Produziert);
     else if(!ppsInstanz::getProduktionsInstanz(artikel)->ProduziertSelbst())
           ppsInstanz::getProduktionsInstanz(artikel)->Produziert(sp);


     tr.commit();
   } catch(SQLerror &e)
     { std::cout << e <<'\n';}
}

void Lager::raus_aus_lager(ArtikelBase artikel,AuftragBase::mengen_t menge,int uid)
{
  ManuProC::Trace _t(ManuProC::Tracer::Auftrag, __FUNCTION__,
     "Artikel=",artikel,"Menge=",menge);
  assert(menge>=0);
  try{
    assert((*this)->ProduziertSelbst());

     ManuProC::st_produziert sp(artikel,menge,uid);
     (*this)->Lager_abschreiben(sp);

   } catch(SQLerror &e) { std::cout << e <<'\n';}
}


bool Lager::dispo_auftrag_aendern(ArtikelBase artikel,AuftragBase::mengen_t menge)
{
  ManuProC::Trace _t(ManuProC::Tracer::Auftrag, __FUNCTION__,
     "Artikel=",artikel,"Menge=",menge);
   AuftragBase da(*this,AuftragBase::dispo_auftrag_id);
   int znr=-1,newznr=-1;
   AuftragBase::mengen_t oldmenge;
   bool alt=da.existEntry(artikel,Lager::Lagerdatum(),znr,newznr,oldmenge,OPEN);
   if(alt)
     {
      AuftragBase::mengen_t mt=AufEintragBase(da,znr).updateStkDiffBase__(getuid(),menge);
      assert(mt==menge);
     }
   else
     {
      Auftrag A(da);
      A.push_back(menge,Lager::Lagerdatum(),artikel,OPEN,getuid(),false);
     }
  return alt;
}


void Lager::menge_neu_verplanen(int uid,
              const ArtikelBase& artikel,AuftragBase::mengen_t menge,
              const ManuProC::Auftrag::Action reason) throw(SQLerror)
{
  ManuProC::Trace _t(ManuProC::Tracer::Auftrag, __FUNCTION__,"Instanz=",*this,
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


std::vector<class LagerInhalt> Lager::LagerInhalt()  const
{
  return LagerInhalt_(ArtikelBase());
} 
  
class LagerInhalt Lager::LagerInhalt(const ArtikelBase& artikel) const
{
  std::vector<class LagerInhalt> L=LagerInhalt_(artikel);
  if(L.empty()) return class LagerInhalt(artikel,0,0,0,0);
  return *(L.begin());
}
        
void Lager::LagerInhaltSum(std::vector<class LagerInhalt>& LI) 
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

        