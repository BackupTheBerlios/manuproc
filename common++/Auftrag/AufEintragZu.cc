// $Id: AufEintragZu.cc,v 1.10 2003/02/14 09:53:53 christof Exp $
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


AufEintragZu::list_t AufEintragZu::get_Referenz_list_id(const AuftragBase::ID id,bool kinder) const throw(SQLerror)
{
   ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this,"Id=",id,"Kinder=",kinder);
   std::list<st_reflist> L=get_Referenz_list(*this,kinder,list_ohneArtikel); // kinder/* oder false? */);
   std::list<st_reflist> N=select_Id(id,L);
   if(N.empty() && kinder) // Für die Reparatur; ein Pfeil könnte ins nichts zeigen ...
    {
      L=get_Referenz_list_without_child();
      N=select_Id(id,L);
    }   
   return N;
}

AufEintragZu::list_t AufEintragZu::select_Id(const AuftragBase::ID id,const std::list<st_reflist> &L) const
{
   std::list<st_reflist> N;
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

 std::list<st_reflist> L=get_Referenz_list(*this,kinder,list_ohneArtikel);
 std::list<st_reflist> N;
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
        std::list<st_reflist> N_=AufEintragZu(i->AEB).get_Referenz_list_geplant(kinder);
        N.splice(N.end(),N_);
      }
   }
*/
 return N;
}
      


AufEintragZu::list_t AufEintragZu::get_Referenz_listFull(bool kinder,bool nur_ende) const throw(SQLerror)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this,"Kinder=",kinder,"NurEnde=",nur_ende);
 std::list<st_reflist> tv=get_Referenz_list(*this,kinder,list_ohneArtikel);
 std::list<st_reflist> vaeb;
 std::list<st_reflist> tvxx;
reloop:
 tv.splice(tv.end(),tvxx);
 for (std::list<st_reflist>::iterator i=tv.begin();i!=tv.end();++i)
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
  std::list<st_reflist> URA=get_Referenz_list(*this,false,list_ohneArtikel);
 //NEU
  std::list<st_reflist> L;
  for(std::list<st_reflist>::const_iterator i=URA.begin();i!=URA.end();++i)
   {
     std::list<st_reflist> l=AufEintragZu(i->AEB).get_Referenz_list(i->AEB,kinder,list_ohneArtikel);
     if(kinder) // sich selber aus der Liste entfernen
        for (std::list<st_reflist>::iterator j=l.begin();j!=l.end();++j)
           if(j->AEB==*this) {L.erase(j); break;}
     L.merge(l);
   }
  return L; 
}




AuftragBase::mengen_t AufEintragZu::verteileMenge(std::list<st_reflist> L, mengen_t menge,bool add)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this,"Menge=",menge,"Add=",add);
  for(std::list<st_reflist>::const_iterator i=L.begin();i!=L.end();++i)
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
  AufEintragZu::list_t L=AufEintragZu(*this).get_Referenz_listFull(false);
  for (AufEintragZu::list_t::const_iterator i=L.begin();i!=L.end();++i)
    V.push_back(i->AEB);
  return V;
}

