// $Id: AufEintragBase.cc,v 1.26 2002/07/15 15:37:52 christof Exp $
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
#include <Aux/Transaction.h>
//#include <Auftrag/auftrag_status.h>
#include <Auftrag/AuftragsBaum.h> 
//#include <Aux/Changejournal.h>
#include <Auftrag/AufEintragZu.h>
#include <Artikel/ArtikelBaum.h>
#include <Artikel/ArtikelStamm.h>
#include "AufEintrag.h"
#ifdef MABELLA_EXTENSIONS
#include<Lager/FertigWaren.h>
#include<Artikel/ArtikelBase.h>
#endif



std::ostream &operator<<(std::ostream &o,const AufEintragBase &ae)
{
 o<<ae.Instanz()->Name()<<'('<<ae.Instanz()->Id()<<") "<<ae.Id()<<' '<<ae.ZNr();
 return o;
}

void AufEintragBase::calculateProzessInstanz()
{
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



void AufEintragBase::Planen(int uid,std::vector<std::AufEintrag> LAE,mengen_t menge,
      const AuftragBase &zielauftrag,const ManuProC::Datum &datum)
{
 int znr=AufEintragBase::none_znr;
 ArtikelBase artikel=ArtikelBase::none_id;
 Transaction tr;
 for(std::vector<std::AufEintrag>::iterator i=LAE.begin();i!=LAE.end();++i)
  {
    int znrtmp;
    if(i->getRestStk()<=menge)
     {
       menge-=i->getRestStk();
       znrtmp=i->Planen(uid,i->getRestStk(),true,zielauftrag,datum);
     }
    else
     {
      if(menge>0)
        znrtmp=i->Planen(uid,menge,true,zielauftrag,datum);
     }
    if(znr==AufEintragBase::none_znr) znr=znrtmp;
    else assert(znr==znrtmp);
    if(artikel==ArtikelBase::none_id) artikel=i->Artikel();
    else assert(artikel==i->Artikel());
  }
 if(menge>0) // Überplanung
  {
    AufEintragBase(zielauftrag,znr).PlanenDispo(uid,artikel,menge,datum);
  }
 tr.commit();
}


void AufEintragBase::PlanenDispo(int uid,const ArtikelBase& artikel,mengen_t menge,const ManuProC::Datum &datum)
{
   AuftragBase dispoAB(Instanz(),dispo_auftrag_id);
//   int znr=dispoAB.insertNewEntry(menge,datum,artikel,OPEN,uid,false);
   int znr=dispoAB.tryUpdateEntry(menge,datum,artikel,OPEN,uid,*this,false,true);
   AufEintragBase dispoAEB(dispoAB,znr);

//   AufEintragZu(dispoAEB).Neu(*this,menge);
   updateStkDiffBase__(uid,menge);
  
   AufEintrag AE(dispoAEB);
   dispoAEB.InstanzAuftraegeAnlegen(AE.Artikel(),znr,AE.getLieferdatum(),AE.getEntryStatus(),uid,menge);
}


int AufEintragBase::split_zuordnungen_to(mengen_t menge,ManuProC::Datum datum,
                        ArtikelBase artikel,AufStatVal status,int uid,
                        bool dispoplanung)
{
  assert(Id()==AuftragBase::ungeplante_id);
  // ElternListe holen
  std::list<AufEintragZu::st_reflist> L=AufEintragZu(*this).get_Referenz_list(*this,false);
  int znr=none_znr;
  assert(!L.empty());
  for(std::list<AufEintragZu::st_reflist>::iterator i=L.begin();i!=L.end();++i)
   {
    mengen_t M;
    if(i->Menge > menge) M=menge;
    else M=i->Menge;
    if(!dispoplanung)
       znr=tryUpdateEntry(M,datum,artikel,status,uid,i->AEB,false);
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

#include <Lager/Lager.h>
#include <Lager/Lager_Vormerkungen.h>

void AufEintragBase::move_menge_to_dispo_zuordnung_or_lager(mengen_t menge,ArtikelBase artikel,int uid,bool produziert)
{
 std::list<AufEintragZu::st_reflist> K=AufEintragZu(*this).get_Referenz_list(*this,true);
 for (std::list<AufEintragZu::st_reflist>::const_iterator i=K.begin();i!=K.end();++i)
  {
    if(i->AEB.Id()==AuftragBase::ungeplante_id) continue;
    AufEintrag GeplanterAE(i->AEB);
    AuftragBase::mengen_t M;
    if(GeplanterAE.getRestStk()>=menge)  M=menge;
    else M=GeplanterAE.getRestStk();
    
    AufEintragZu(*this).setMengeDiff__(i->AEB,-M);

    if(Instanz()->LagerInstanz())
     {
      mengen_t mt=i->AEB.updateStkDiffBase__(uid,-M);

      H_Lager L(Instanz());
      L->dispo_auftrag_aendern(artikel,M);
      Lager_Vormerkungen::freigegeben_menge_neu_verplanen(Instanz(),artikel,M,uid,produziert);

      assert(mt==mengen_t(-M));
     }
    else
     {
       std::list<AufEintragZu::st_reflist> E=AufEintragZu(i->AEB).get_Referenz_list(i->AEB);
       for (std::list<AufEintragZu::st_reflist>::const_iterator j=E.begin();j!=E.end();++j)
        {
         if(j->AEB.Id()!=AuftragBase::dispo_auftrag_id) continue;
         AufEintragZu(j->AEB).setMengeDiff__(i->AEB,M);
         j->AEB.updateStkDiffBase__(uid,M);
        }
     }
    menge-=M;
    if(menge==AuftragBase::mengen_t(0)) return;
  }
}
