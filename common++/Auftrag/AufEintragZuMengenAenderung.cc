// $Id: AufEintragZuMengenAenderung.cc,v 1.2 2002/11/29 07:17:22 thoma Exp $
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
  ManuProC::Trace _t(ManuProC::Tracer::Auftrag, __FUNCTION__,
      "Old_AEB=",old_parent,"New_AEB=",new_parent,"Menge=",menge);
  std::list<AufEintragZu::st_reflist> K=AufEintragZu(old_parent).get_Referenz_list(old_parent,true);
  for(std::list<AufEintragZu::st_reflist>::const_iterator i=K.begin();i!=K.end();++i)
   {
     AufEintragZu(new_parent).Neu(i->AEB,menge); // Neu versucht zunächst ein update
     AufEintragZu(old_parent).Neu(i->AEB,-menge); 
   }
}


void AufEintragZuMengenAenderung::increase_parents__reduce_assingments(const int uid,
                     const AufEintragBase &child_aeb,const AuftragBase::mengen_t &menge) throw(SQLerror)
{
  ManuProC::Trace _t(ManuProC::Tracer::Auftrag, __FUNCTION__,"AEB=",child_aeb,"Menge=",menge);
  std::list<AufEintragZu::st_reflist> L=AufEintragZu(child_aeb).get_Referenz_list(child_aeb);
  AuftragBase::mengen_t M=menge;

  for(std::list<AufEintragZu::st_reflist>::iterator j=L.begin();j!=L.end();++j)
    {
      AuftragBase::mengen_t m=AuftragBase::min(j->Menge,M);

      AufEintragZu(j->AEB).setMengeDiff__(child_aeb,-m);
      AufEintrag(j->AEB).updateStkDiff__(uid,m,true,ManuProC::Auftrag::r_Produziert);
      M-=m;
      if(M==AuftragBase::mengen_t(0)) break;
    }         
}

