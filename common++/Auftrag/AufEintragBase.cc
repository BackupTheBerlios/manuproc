// $Id: AufEintragBase.cc,v 1.36 2003/01/08 17:40:43 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

#include <Auftrag/AufEintragBase.h>
#include "AufEintragBase.h"
#include <Misc/Transaction.h>
#include <Auftrag/AuftragsBaum.h> 
#include <Auftrag/AufEintragZu.h>
#include <Artikel/ArtikelBaum.h>
#include <Artikel/ArtikelStamm.h>
#include <Artikel/ArtikelImLager.h>
#include "AufEintrag.h"
#ifdef MABELLA_EXTENSIONS
#include<Lager/FertigWaren.h>
#include<Artikel/ArtikelBase.h>
#endif
#include <Misc/Trace.h>
#include<Misc/FetchIStream.h>
#include <Auftrag/AufEintragZuMengenAenderung.h>

FetchIStream& operator>>(FetchIStream& is,AufEintragBase &aeb)
{
  int a,b,c;
  is >> a>>b>>c;
  cH_ppsInstanz I((ppsInstanz::ID(a)));
  AuftragBase ab(I,b);
  aeb=AufEintragBase(ab,c);
  return is;
}


std::ostream &operator<<(std::ostream &o,const AufEintragBase &ae)
{
 o<<ae.Instanz()->Name()<<'('<<ae.Instanz()->Id()<<") "<<ae.Id()<<' '<<ae.ZNr();
 return o;
}

std::string AufEintragBase::str() const
{
 return Instanz()->Name()+"("+itos(Instanz()->Id())+")|"+itos(Id())+"|"+itos(ZNr());
}



void AufEintragBase::calculateProzessInstanz()
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this);
  assert (Instanz()==ppsInstanzID::Kundenauftraege);
  AuftragsBaum AB(*this,true);
  int anz=0;
  for(AuftragsBaum::const_iterator i=AB.begin();i!=AB.end();++i)
   {
     if(i->AEB2.Id()==0) continue; // 0 = ungeplante Aufträge
     if(AufEintrag(i->AEB2).getStueck() == i->menge)
       ++anz;
   }
  setMaxPlanInstanz(anz);
}   

int AufEintragBase::split_zuordnungen_to(mengen_t menge,ManuProC::Datum datum,
                        ArtikelBase artikel,AufStatVal status,int uid,
                        bool dispoplanung)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,str(),
   "Artikel=",artikel,
   "Menge=",menge,"Status=",status,"DispoPlanung=",dispoplanung);

  assert(Id()==AuftragBase::ungeplante_id);
  // ElternListe holen
  std::list<AufEintragZu::st_reflist> L=AufEintragZu(*this).get_Referenz_list(*this,false);
  int znr=none_znr;
  assert(!L.empty());
  for(std::list<AufEintragZu::st_reflist>::iterator i=L.begin();i!=L.end();++i)
   {
    mengen_t M=min(i->Menge,menge);
    if(!dispoplanung)
       znr=tryUpdateEntry(M,datum,artikel,status,uid,i->AEB);
    else
      { int dummy;
        assert(existEntry(artikel,datum,znr,dummy,menge,status));
        AufEintragBase aeb_verplant(*this,znr);
        AufEintragZu(i->AEB).setMengeDiff__(aeb_verplant,M);
      }
    AufEintragZu(i->AEB).setMengeDiff__(*this,-M);
    menge-=M;
    if(M==mengen_t(0)) break;
   }
 return znr;
}

void AufEintragBase::ExistMenge(const std::string &s) const
{
 try{AufEintrag AE(*this);
     std::cout << *this<<" existiert mit " <<AE.getStueck()<<" bestellt und "
          <<AE.getGeliefert()<<" geliefert ("<<s<<")\n";
   }catch(...){std::cout << *this<<"\texistiert noch nicht ("<<s<<")\n";}
}


#if 0
void AufEintragBase::vormerken_oder_bestellen(int uid,const AuftragBase::mengen_t &vormerkmenge,
            AuftragBase::mengen_t bestellmenge,
            const ArtikelBase &artikel,const Petig::Datum &lieferdatum,
            std::vector<AufEintrag> dispo_auftrag,
            AuftragBase::st_tryUpdateEntry st_bool)  const throw(SQLerror)
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,"Vormerkmenge=",vormerkmenge,
      "Bestellmenge=",bestellmenge);
  assert(vormerkmenge<=bestellmenge);

  // 1. Fall: Reicht die Menge, die wir bekommen haben aus?
  if(vormerkmenge>mengen_t(0))
   {
     artikel_vormerken_oder_schnappen(false,vormerkmenge,artikel,uid,ManuProC::Auftrag::r_Anlegen,dispo_auftrag);
   }
  bestellmenge-=vormerkmenge;
  if(bestellmenge==AuftragBase::mengen_t(0)) return;

  // 2. Fall: Haben wir vorgemerkte Menge mit späterem Lieferdatum
  //          in einer LagerInstanz die wir uns schnappen können?
  if(instanz->LagerInstanz())
   {
     ArtikelImLager AIL(Instanz(),artikel,lieferdatum);
     AuftragBase::mengen_t menge_vorgemerkt = AIL.getMengePlan();
     if(menge_vorgemerkt>mengen_t(0))
      {     
        AuftragBase::mengen_t Mv=min(bestellmenge,menge_vorgemerkt);
        artikel_vormerken_oder_schnappen(true,Mv,artikel,uid,ManuProC::Auftrag::r_Planen,AIL.getPlanAuftraege());
        bestellmenge-=Mv;
        if(bestellmenge==AuftragBase::mengen_t(0)) return;
      }     
   }

  // 3. Fall: Wir müssen weiter bestellen
  if(Instanz()->LagerInstanz())
    {
      AuftragBase AB(ppsInstanz::getProduktionsInstanz(artikel),AuftragBase::ungeplante_id);
      AB.tryUpdateEntry(bestellmenge,lieferdatum,artikel,OPEN,uid,*this);
    }
  else
    { 
      InstanzAuftraegeAnlegen(*this,bestellmenge,uid,st_bool.automatisch_geplant);
    }
}

void AufEintragBase::artikel_vormerken_oder_schnappen(bool schnappen,AuftragBase::mengen_t menge,
      const ArtikelBase &artikel,int uid,ManuProC::Auftrag::Action reason,
      std::vector<AufEintrag> dispo_auftrag) const
{
   ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,"Schnappen=",schnappen,"Menge=",menge,
      "Artikel=",artikel,
      "Reason=",reason,"SizeOfDispoAufträgen=",dispo_auftrag.size());

  if(menge==AuftragBase::mengen_t(0)) return ;

  // Bei den 2er (dispo_auftrag_id) die verfügbare Menge reduzieren
  // Bei den 1er (plan_auftrag_id) die  verfügbare Menge wegnehmen 
  AuftragBase::mengen_t abmenge=menge;
  for(std::vector<AufEintrag>::reverse_iterator i=dispo_auftrag.rbegin();i!=dispo_auftrag.rend();++i)
   {
     if(schnappen) assert(i->Id()==AuftragBase::plan_auftrag_id);
     else          assert(i->Id()==AuftragBase::dispo_auftrag_id);
     AuftragBase::mengen_t use_menge = min(i->getRestStk(),abmenge);
     AuftragBase::mengen_t mt=i->updateStkDiffBase__(uid,-use_menge);
     assert(mt==-use_menge);

     if(schnappen)
      {
         AufEintragZuMengenAenderung::increase_parents__reduce_assingments(uid,*i,use_menge);
      }
     else
      {  if(!Instanz()->LagerInstanz() && !Instanz()->PlanungsInstanz())
            AufEintragZuMengenAenderung::change_parent(uid,*i,*this,use_menge);
      }
     abmenge-=use_menge;
     if(abmenge==AuftragBase::mengen_t(0)) break;
     assert(abmenge>AuftragBase::mengen_t(0));   
   }
  if(Instanz()->LagerInstanz()) // neuen 1er anlegen
   {
     if(schnappen) assert(reason==ManuProC::Auftrag::r_Planen);
     AufEintrag AE(*this);
     AE.Planen(uid,menge,AuftragBase(Instanz()->Id(),AuftragBase::plan_auftrag_id),
            AE.getLieferdatum(),reason); 
   }

}

#endif
