// $Id: AufEintragZu.cc,v 1.11 2003/02/15 22:53:21 christof Exp $
/*  libcommonc++: ManuProC's main OO library
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

#include <Auftrag/AufEintragZu.h>
#include <Misc/Transaction.h>
#include <Auftrag/auftrag_status.h>
#include <Misc/Changejournal.h>
#include <Artikel/ArtikelStamm.h>
#include <Artikel/ArtikelBezeichnung.h>
#include <Misc/FetchIStream.h>
#include <Misc/Trace.h>

// was tut das eigentlich ? CP
AufEintragZu::list_t AufEintragZu::get_Referenz_list_id(const AuftragBase::ID id,bool kinder,bool artikel) const throw(SQLerror)
{
   ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this,"Id=",id,"Kinder=",kinder);
   list_t L=get_Referenz_list(*this,kinder,artikel); // kinder/* oder false? */);
   list_t N=select_Id(id,L);
   if(N.empty() && kinder) // Für die Reparatur; ein Pfeil könnte ins nichts zeigen ...
    {
      L=get_Referenz_list_without_child();
      N=select_Id(id,L);
    }   
   return N;
}

AufEintragZu::list_t AufEintragZu::select_Id(const AuftragBase::ID id,const list_t &L) const
{
   list_t N;
   for(AufEintragZu::list_t::const_iterator i=L.begin();i!=L.end();++i)
       if(i->AEB.Id()==id) 
         N.push_back(*i);
   return N;
}

AufEintragZu::list_t AufEintragZu::get_Referenz_list_geplant(bool kinder) const throw(SQLerror)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this,"Kinder=",kinder);

 if(Instanz()->LagerInstanz())
     return get_Referenz_list_id(AuftragBase::plan_auftrag_id,kinder);

 list_t L=get_Referenz_list(*this,kinder,list_ohneArtikel);
 list_t N;
 for(AufEintragZu::list_t::const_iterator i=L.begin();i!=L.end();++i)
  {
     if(i->AEB.Id()!=AuftragBase::dispo_auftrag_id &&
        i->AEB.Id()!=AuftragBase::ungeplante_id &&
        i->AEB.Id()!=AuftragBase::invalid_id ) 
       N.push_back(*i);
  }
/*
 else 
   { assert(kinder);
     assert(L.empty() || L.size()==1);
     for(AufEintragZu::list_t::const_iterator i=L.begin();i!=L.end();++i)
      {
        list_t N_=AufEintragZu(i->AEB).get_Referenz_list_geplant(kinder);
        N.splice(N.end(),N_);
      }
   }
*/
 return N;
}
      


AufEintragZu::list_t AufEintragZu::get_Referenz_listFull(bool kinder,bool nur_ende) const throw(SQLerror)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this,"Kinder=",kinder,"NurEnde=",nur_ende);
 list_t tv=get_Referenz_list(*this,kinder,list_ohneArtikel);
 list_t vaeb;
 list_t tvxx;
reloop:
 tv.splice(tv.end(),tvxx);
 for (list_t::iterator i=tv.begin();i!=tv.end();++i)
   {
     tvxx=get_Referenz_list(i->AEB,kinder,list_ohneArtikel);
     if(nur_ende)
      {
        if (tvxx.empty()) vaeb.splice(vaeb.end(),tv,i) ;
        else tv.erase(i) ; 
      }
     else vaeb.splice(vaeb.end(),tv,i) ;
     goto reloop;
   }
 return vaeb;
}


AufEintragZu::list_t AufEintragZu::get_Referenz_list_for_geplant(bool kinder) const throw(SQLerror)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this,"Kinder=",kinder);
  // Ungeplante Referenz Aufträge
  list_t URA=get_Referenz_list(*this,false,list_ohneArtikel);
 //NEU
  list_t L;
  for(list_t::const_iterator i=URA.begin();i!=URA.end();++i)
   {
     list_t l=AufEintragZu(i->AEB).get_Referenz_list(i->AEB,kinder,list_ohneArtikel);
     if(kinder) // sich selber aus der Liste entfernen
        for (list_t::iterator j=l.begin();j!=l.end();++j)
           if(j->AEB==*this) {L.erase(j); break;}
     L.merge(l);
   }
  return L; 
}




AuftragBase::mengen_t AufEintragZu::verteileMenge(list_t L, mengen_t menge,bool add)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this,"Menge=",menge,"Add=",add);
  for(list_t::const_iterator i=L.begin();i!=L.end();++i)
   {
     if(menge==AuftragBase::mengen_t(0)) return menge;
     AuftragBase::mengen_t M=AuftragBase::min(menge,i->Menge);
//     if(i->Menge >= menge) M=menge;
//     else M=i->Menge;
     if(add)  setMengeDiff__(i->AEB,M)  ;
     else     setMengeDiff__(i->AEB,-M) ;
     menge-=M;
   }
  if(menge!=AuftragBase::mengen_t(0)) 
      std::cerr << "WARNING: 'AufEintragZu::verteileMenge' Restmenge = "<<menge<<'\n';
  return menge;
}

std::vector<AufEintragBase> AufEintragZu::getKundenAuftragV() const
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__);
  std::vector<AufEintragBase> V;
  list_t L=AufEintragZu(*this).get_Referenz_listFull(false);
  for (AufEintragZu::list_t::const_iterator i=L.begin();i!=L.end();++i)
    V.push_back(i->AEB);
  return V;
}

AufEintragZu::map_t AufEintragZu::get_Kinder_nach_Artikel(const AufEintragBase &aeb,bool kinder)
{  list_t KindListeU(AufEintragZu::get_Referenz_list(aeb,kinder,AufEintragZu::list_Artikel));
   map_t MapArt;

   for(list_t::const_iterator i=KindListeU.begin();i!=KindListeU.end();++i)
    {
      // Nach dem Planen kann es zu einem Auftrag mehrere Kindaufträge 
      // mit demselben Artikel geben, nur bei EINEM darf die Menge geändert
      // werden
      MapArt[i->Art].push_back(*i);
     }
   return MapArt;
}
