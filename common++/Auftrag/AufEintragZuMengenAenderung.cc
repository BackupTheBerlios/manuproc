// $Id: AufEintragZuMengenAenderung.cc,v 1.4 2002/12/17 13:55:32 thoma Exp $
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
#include <Auftrag/AufEintrag.h>  
#include <Auftrag/AufEintragZuMengenAenderung.h>
#include <Aux/Trace.h>


void AufEintragZuMengenAenderung::change_parent(const int uid,
                                                const AufEintragBase &old_parent,
                                                const AufEintragBase &new_parent,
                                                const AuftragBase::mengen_t &menge) throw(SQLerror)
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,
      "Old_AEB=",old_parent,"New_AEB=",new_parent,"Menge=",menge);
  std::list<AufEintragZu::st_reflist> K=AufEintragZu(old_parent).get_Referenz_list(old_parent,true);
  for(std::list<AufEintragZu::st_reflist>::const_iterator i=K.begin();i!=K.end();++i)
   {
     AufEintragZu(new_parent).Neu(i->AEB,menge); // Neu versucht zunächst ein update
     AufEintragZu(old_parent).Neu(i->AEB,-menge); 
   }
}


void AufEintragZuMengenAenderung::increase_parents__reduce_assingments(const int uid,
                     const AufEintragBase &child_aeb,AuftragBase::mengen_t menge) throw(SQLerror)
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,"AEB=",child_aeb,"Menge=",menge);
  std::list<AufEintragZu::st_reflist> L=AufEintragZu(child_aeb).get_Referenz_list(child_aeb);
  for(std::list<AufEintragZu::st_reflist>::iterator j=L.begin();j!=L.end();++j)
    {
      AuftragBase::mengen_t m=AuftragBase::min(j->Menge,menge);

      AufEintragZu(j->AEB).setMengeDiff__(child_aeb,-m);
      AufEintrag(j->AEB).updateStkDiff__(uid,m,true,ManuProC::Auftrag::r_Produziert);
      menge-=m;
      if(menge==AuftragBase::mengen_t(0)) break;
    }         
}

void AufEintragZuMengenAenderung::Change_Zuordnung_to_Children(const bool child,const AufEintrag &AE,
                        const AuftragBase::mengen_t &menge) throw(SQLerror)
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,"AE=",AE,"Menge=",menge,"Child=",child);
  const bool reduce = (menge<0);
  AuftragBase::mengen_t Me=menge;
  if(reduce) Me*=-1;
  // K ist nach Instanzen sortiert
  std::list<AufEintragZu::st_reflist>  K=AufEintragZu(AE).get_Referenz_list_ungeplant();
  std::map<cH_ppsInstanz,AuftragBase::mengen_t> IM;
  for(std::list<AufEintragZu::st_reflist>::const_iterator i=K.begin();i!=K.end();++i)
   {
     // Wenn diese Instanz schon behandelt wurde und keine Menge mehr über ist
     if(IM.find(i->AEB.Instanz())!=IM.end() && 
        IM.find(i->AEB.Instanz())->second==Me) continue;

     AuftragBase::mengen_t M;
     ArtikelBaum::faktor_t F=ArtikelBaum(AE.Artikel()).Faktor(i->Art);
     if (reduce) M=-AuftragBase::min(i->Menge,Me*F);
     else        M=Me*F;
     IM[i->AEB.Instanz()] += F*M.abs();
cout << "Change_Zuordnung_to_Children: "<<AE<<'\t'<<i->AEB<<'\t'<<M;
     AufEintragZu(AE).setMengeDiff__(i->AEB,M);
cout <<"Faktor="<<F<<'\t'<<M<<'\n';
   }
}
