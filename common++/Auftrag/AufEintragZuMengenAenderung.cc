// $Id: AufEintragZuMengenAenderung.cc,v 1.9 2002/12/24 08:25:22 thoma Exp $
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
     AufEintragZu(new_parent).Neu(i->AEB,menge); // Neu versucht zun�chst ein update
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
//cout << child_aeb<<'\t'<<j->AEB<<'\t'<<menge<<'\t'<<j->Menge<<'\n';
      AuftragBase::mengen_t m=AuftragBase::min(j->Menge,menge);

      AufEintragZu(j->AEB).setMengeDiff__(child_aeb,-m);

      std::list<AufEintragZu::st_reflist> L2=AufEintragZu(j->AEB).get_Referenz_list_ungeplant();
      for(std::list<AufEintragZu::st_reflist>::iterator k=L2.begin();k!=L2.end();++k)
       {
         if(k->Art!=AufEintrag(child_aeb).Artikel()) continue;
         AufEintragZu(j->AEB).setMengeDiff__(k->AEB,m);
         AufEintrag(k->AEB).updateStkDiff__(uid,m,true,j->AEB,ManuProC::Auftrag::r_Planen);
         break;
       }   
/*
      AufEintrag cAE(child_aeb);
      // Den er suchen
      int znr,dummy;
      AufEintragBase::mengen_t M_;
      AuftragBase AB_(child_aeb.Instanz(),AuftragBase::ungeplante_id);
      AB_.existEntry(cAE.Artikel(),cAE.getLieferdatum(),znr,dummy,M_,OPEN);
//      AufEintrag(j->AEB).updateStkDiff__(uid,m,true,ManuProC::Auftrag::r_Produziert);
      AufEintragBase AEB_(AB_,znr);
      AufEintrag(AEB_).updateStkDiff__(uid,m,true,AufEintragBase(),ManuProC::Auftrag::r_Produziert);
      AufEintragZu(j->AEB).setMengeDiff__(AEB_,m);
*/
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
     // Wenn diese Instanz schon behandelt wurde und keine Menge mehr �ber ist
     if(IM.find(i->AEB.Instanz())!=IM.end() && 
        IM.find(i->AEB.Instanz())->second==Me) continue;

     AuftragBase::mengen_t M;
     ArtikelBaum::faktor_t F=ArtikelBaum(AE.Artikel()).Faktor(i->Art);
     if (reduce) M=-AuftragBase::min(i->Menge,Me*F);
     else        M=Me*F;
     IM[i->AEB.Instanz()] += F*M.abs();
//cout << "Change_Zuordnung_to_Children: "<<AE<<'\t'<<i->AEB<<'\t'<<M;
     AufEintragZu(AE).setMengeDiff__(i->AEB,M);
//cout <<"Faktor="<<F<<'\t'<<M<<'\n';
   }
}


void AufEintragZuMengenAenderung::move_zuordnung_zu_geplantem(const int uid,
         AufEintrag &AE0er, AufEintrag &AE1er,
         AuftragBase::mengen_t menge,
         ManuProC::Auftrag::Action reason) throw(SQLerror)
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,"AE0er=",AE0er,"AE1er=",AE1er,"Menge=",menge);
  std::list<AufEintragZu::st_reflist> L=AufEintragZu(AE0er).get_Referenz_list(AE0er);
  for(std::list<AufEintragZu::st_reflist>::reverse_iterator i=L.rbegin();i!=L.rend();++i)
   {
    AuftragBase::mengen_t M=AuftragBase::min(i->Menge,menge);
    AufEintragZu(i->AEB).setMengeDiff__(AE0er,-M);
    AufEintragZu(i->AEB).Neu(AE1er,M);
    menge-=M;
    if(!menge) break;
   }
}

void AufEintragZuMengenAenderung::reduce_zuordung_to_20000er_from_2er(const int uid,
         const AufEintrag &AE2er,AuftragBase::mengen_t menge) throw(SQLerror)
{
  std::list<AufEintragZu::st_reflist> L=AufEintragZu(AE2er).get_Referenz_list_geplant();
  for(std::list<AufEintragZu::st_reflist>::reverse_iterator i=L.rbegin();i!=L.rend();++i)
   {
    AuftragBase::mengen_t M=AuftragBase::min(i->Menge,menge);
    AufEintragZu(AE2er).setMengeDiff__(i->AEB,-M);
    menge-=M;
    if(!menge) break;
   }
}
                  