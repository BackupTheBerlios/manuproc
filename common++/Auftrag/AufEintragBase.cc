// $Id: AufEintragBase.cc,v 1.20 2002/06/20 06:54:51 christof Exp $
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

#if 0
#ifdef PETIG_EXTENSIONS
void AufEintragBase::abschreiben_fuer_Instanzen(mengen_t menge) const throw(SQLerror)
{
   int uid=0;
   // Kindaufträge
   std::list<AufEintragZu::st_reflist> L=AufEintragZu(*this).get_Referenz_list_ungeplant();
   for(std::list<AufEintragZu::st_reflist>::iterator i=L.begin();i!=L.end();++i)
    {
      ArtikelBaum::menge_t f=ArtikelBaum(AufEintrag(*this).Artikel()).Faktor(i->Art);
      AuftragBase::mengen_t newmenge=menge*f;
      std::list<AufEintragZu::st_reflist> Lg=AufEintragZu(i->AEB).get_Referenz_list_geplant();
//      AuftragBase::mengen_t verplante_menge=0;
      for(std::list<AufEintragZu::st_reflist>::iterator j=Lg.begin();j!=Lg.end();++j)
       {
         AufEintrag AE(j->AEB);
//         verplante_menge+=AE.getStueck();
         if( AE.getGeliefert()<AE.getStueck() ) // Möglicherweise hat 'auslagern' schon abgeschrieben
            j->AEB.abschreiben(AE.getRestStk());
       }
cout << "Menge bei "<<i->AEB<<" reduzieren von "<<AufEintrag(i->AEB).getStueck()
<<" um "<<-(newmenge/*-verplante_menge*/)<<'\n';
      AufEintrag(i->AEB).updateStkDiff__(uid,-(newmenge/*-verplante_menge*/),false);    
      i->AEB.abschreiben_fuer_Instanzen(newmenge);
    }
/*
   // Elternaufträge (mit dispo_auftrag_id)
   L=AufEintragZu(*this).get_Referenz_list(*this);
   for(std::list<AufEintragZu::st_reflist>::iterator i=L.begin();i!=L.end();++i)
    {
      if(i->AEB.Id()!=dispo_auftrag_id) continue;
      i->AEB.abschreiben(menge);
    }
*/
}
#endif
#endif


void AufEintragBase::calculateProzessInstanz()
{
//cout << "BAUM\n";
  assert (Instanz()==ppsInstanzID::Kundenauftraege);
  AuftragsBaum AB(*this,true);
  int anz=0;
  for(AuftragsBaum::const_iterator i=AB.begin();i!=AB.end();++i)
   {
//cout << "Baum\t"<<i->AEB2.Instanz()->Name()<<' '<<i->AEB2.Instanz()->Id()
//<<' ' <<i->AEB2.Id()
//<<' '<<i->AEB2.ZNr()<<'\n';
     if(i->AEB2.Id()==0) continue; // 0 = ungeplante Aufträge
     if(AufEintrag(i->AEB2).getStueck() == i->menge)
       ++anz;
   }
  setMaxPlanInstanz(anz);
}   



void AufEintragBase::Planen(int uid,std::vector<std::AufEintrag> LAE,mengen_t menge,
      const AuftragBase &zielauftrag,const Petig::Datum &datum)
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
       znrtmp=i->Planen(uid,i->getRestStk(),zielauftrag,datum);
     }
    else
     {
      if(menge>0)
        znrtmp=i->Planen(uid,menge,zielauftrag,datum);
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


void AufEintragBase::PlanenDispo(int uid,const ArtikelBase& artikel,mengen_t menge,const Petig::Datum &datum)
{
   AuftragBase dispoAB(Instanz(),dispo_auftrag_id);
   int znr=dispoAB.insertNewEntry(menge,datum,artikel,OPEN,uid,false);
   AufEintragBase dispoAEB(dispoAB,znr);

   AufEintragZu(dispoAEB).Neu(*this,menge);
   updateStkDiffBase__(uid,menge);
  
   AufEintrag AE(dispoAEB);
   dispoAEB.InstanzAuftraegeAnlegen(AE.Artikel(),znr,AE.getLieferdatum(),AE.getEntryStatus(),uid,menge);
}


int AufEintragBase::split_zuordnungen_to(mengen_t menge,Petig::Datum datum,
                        ArtikelBase artikel,AufStatVal status,int uid)
{
  // ElternListe holen
  std::list<AufEintragZu::st_reflist> L=AufEintragZu(*this).get_Referenz_list(*this,false);
  int znr=none_znr;
  assert(!L.empty());
  for(std::list<AufEintragZu::st_reflist>::iterator i=L.begin();i!=L.end();++i)
   {
    mengen_t M;
    if(i->Menge > menge) M=menge;
    else M=i->Menge;

    znr=tryUpdateEntry(M,datum,artikel,status,uid,i->AEB);
    AufEintragBase aeb(*this,znr);
    AufEintragZu(i->AEB).setMengeDiff__(*this,-M);
    menge-=M;
    if(M==mengen_t(0)) break;
   }
 return znr;
}

