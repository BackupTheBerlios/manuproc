// $Id: ppsInstanzProduziert.cc,v 1.3 2002/10/24 14:08:59 thoma Exp $
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

#include "ppsInstanz.h"
#include "ppsInstanzProduziert.h"

#include <Artikel/ArtikelBaum.h>
#include <Artikel/ArtikelStamm.h>
#include <Auftrag/sqlAuftragSelector.h>
//#include <Auftrag/selFullAufEntry.h>
#include <Aux/AdminProblems.h>    
#include <Lager/Lager.h>
#include <Aux/Trace.h>


void ppsInstanz::Produziert(ManuProC::st_produziert &P) const
{
 ManuProC::Trace _t(ManuProC::Tracer::Auftrag, __FUNCTION__,
   "Instanz="+Name(),
   "Artikel="+cH_ArtikelBezeichnung(P.artikel)->Bezeichnung(),
   "Menge="+dtos(P.menge),"LfrsId="+itos(P.lfrsid));
 assert(Id()!=ppsInstanzID::None) ; 

 if(P.AE.valid()) // direktes Abschreiben ohne Suche von offenen Aufträgen
     { assert(P.AE.valid());
       P.AE.abschreiben(P.menge);
//cout << P.AE<<'\n';
       P.Reduce_DispoEltern(P.AE,P.menge);      
       rekursion(P);
       return;
     }     
// else if(ProduziertSelbst())  return  ;

//cout << Name()<<'\n';


//(AufEintragBase(ppsInstanz::ID(30),2,3)).ExistMenge("A");

 try{
   if(P.menge>=0)
    {
      AuftragBase::mengen_t restmenge=P.abschreiben_oder_reduzieren(Id(),
                  AuftragBase::PlanId_for(Id()),P.menge);

//cout << Name()<<'\t'<<P.menge<<' '<<restmenge<<'\n';
      if(restmenge>0)
       {
          AuftragBase::mengen_t restmenge2=P.abschreiben_oder_reduzieren(Id(),
                  AuftragBase::ungeplante_id,restmenge);

//(AufEintragBase(ppsInstanz::ID(30),2,3)).ExistMenge("C");

//cout << '\t'<<Name()<<' '<<Id()<<'\t'<<P.menge<<' '<<restmenge<<' '<<restmenge2<<'\n';
#if 0
          if(restmenge2>0) 
           {
cerr << "Restmenge nach Abschreiben bei geplantem Auftrag war "
<<restmenge<<"\nRestmenge nach Reduzieren des ungeplanten Auftrags ist "
<<restmenge2<<"\n   und Was machen wir nun?\n"
           }
#endif
        }       
     P.check_dispo_auftraege(Id());

//(AufEintragBase(ppsInstanz::ID(30),2,3)).ExistMenge("D");
    }
   else //if(menge<0)
    {
      AuftragBase::mengen_t restmenge=P.abschreiben_oder_reduzieren
                                    (Id(),AuftragBase::plan_auftrag_id,P.menge);
      if(restmenge<0) 
       {
         // da bin ich mal gespannt wann das hier fehlschlägt MAT 2.10.02
         assert(restmenge==P.menge);
         assert(Id()==ppsInstanzID::Kundenauftraege) ;
         assert(P.AE.valid());         
         P.AE.abschreiben(restmenge);
       }
    }
  rekursion(P);

  }catch(SQLerror &e) { std::cerr << e<<'\n';}
}

void ppsInstanz::Lager_abschreiben(ManuProC::st_produziert &P) const 
{
 ManuProC::Trace _t(ManuProC::Tracer::Auftrag, __FUNCTION__,
   "Instanz="+Name(),
   "Artikel="+cH_ArtikelBezeichnung(P.artikel)->Bezeichnung(),
   "Menge="+dtos(P.menge),"LfrsId="+itos(P.lfrsid));

  assert(LagerInstanz());
  AuftragBase::mengen_t restmenge=P.abschreiben_oder_reduzieren(Id(),
                                       AuftragBase::plan_auftrag_id,P.menge);
  if(restmenge>0)
   {
     AuftragBase::mengen_t restmenge2=P.abschreiben_oder_reduzieren(Id(),
                  AuftragBase::dispo_auftrag_id,restmenge);
     if(restmenge2!=AuftragBase::mengen_t(0)) P.fehler(*this,P.Mehr_produziert_als_moeglich,
               AuftragBase::dispo_auftrag_id,P.menge,restmenge2) ;
   }
}


void ppsInstanz::rekursion(ManuProC::st_produziert &P) const 
{
 ManuProC::Trace _t(ManuProC::Tracer::Auftrag, __FUNCTION__,
   "Instanz="+Name(),
   "Artikel="+cH_ArtikelBezeichnung(P.artikel)->Bezeichnung(),
   "Menge="+dtos(P.menge),"LfrsId="+itos(P.lfrsid));
 ArtikelStamm as(P.artikel);
 
 if(as.BestellenBei()==ppsInstanzID::None) return;

 if(Id()!=ppsInstanz::getBestellInstanz(P.artikel)->Id())
   {
    if(EinlagernIn()!=ppsInstanzID::None && 
       cH_ppsInstanz(EinlagernIn())->AutomatischEinlagern())
     {
       Lager L(EinlagernIn()); 
       L.rein_ins_lager(P.artikel,P.menge,P.uid);
     }
    else
     {
        cH_ppsInstanz I=as.BestellenBei();
        P.kunde=ManuProC::DefaultValues::EigeneKundenId;
        P.AE=AufEintrag();
        if(I->ProduziertSelbst())  return;
        I->Produziert(P);
        return;
     }
   }   

   ArtikelBaum AB(P.artikel);
   for(ArtikelBaum::iterator i=AB.begin();i!=AB.end();++i)
     {
       cH_ppsInstanz I=ppsInstanz::getBestellInstanz(i->rohartikel);
       ManuProC::st_produziert sp(i->rohartikel,double(i->menge)*P.menge,P.uid);
       if(I->ProduziertSelbst())  return;
       I->Produziert(sp);
     }
}

AuftragBase::mengen_t ManuProC::st_produziert::abschreiben_oder_reduzieren(ppsInstanz::ID instanz,int id,AuftragBase::mengen_t abmenge)
{
 ManuProC::Trace _t(ManuProC::Tracer::Auftrag, __FUNCTION__,
   "Instanz="+cH_ppsInstanz(instanz)->Name(),
   "AuftragID="+itos(id),
   "Menge="+dtos(abmenge));

  assert(id==AuftragBase::handplan_auftrag_id || id==AuftragBase::dispo_auftrag_id ||
         id==AuftragBase::plan_auftrag_id     || id==AuftragBase::ungeplante_id);

  SQLFullAuftragSelector sel;
  if(abmenge>=0) sel=SQLFullAuftragSelector(SQLFullAuftragSelector::sel_Artikel_Planung_id
                                                   (instanz,kunde,artikel,id));
  else sel=SQLFullAuftragSelector(SQLFullAuftragSelector::sel_Artikel_Planung_id
                                                   (instanz,kunde,artikel,id,CLOSED));

  SelectedFullAufList L(sel);

//cerr <<cH_ppsInstanz(instanz)->Name()<<"\tSIZE of "<<id<<": "<< L.size()<<'\n';
  for(SelectedFullAufList::iterator i=L.begin();i!=L.end();++i)
   {
     AuftragBase::mengen_t abschreibmenge;
     if(i->getRestStk() >= abmenge) abschreibmenge = abmenge;
     else                           abschreibmenge = i->getRestStk();

//cerr << cH_ppsInstanz(instanz)->Name()<<' '<<i->Id()<<' '<<i->ZNr()<<"  Art."
//<<i->Artikel().Id()<<' '<<"M = "
//<<abmenge<<' '<<i->getRestStk()<<'\t'<<abschreibmenge<<'\n';

     if(abschreibmenge==AuftragBase::mengen_t(0)) continue;     

     if(i->Id()==AuftragBase::ungeplante_id)
       {
//         SelectedFullAufList::iterator xi=i; ++xi;
//         if(xi==L.end())  abschreibmenge=abmenge ;
         if(i==--L.end()) abschreibmenge=abmenge ;
         AuftragBase zab(i->Instanz(),AuftragBase::plan_auftrag_id);
         int znr=i->Planen(uid,abschreibmenge,true,zab,i->getLieferdatum());
         AufEintragBase zaeb(zab,znr);
         AufEintrag(zaeb).abschreiben(abschreibmenge,lfrsid);
//cerr << "Produziert:: Planen "<<zaeb<<" und Abschreiben bei "<<*i
//<<" Menge:"<<abschreibmenge<<" alte Menge: "<<abmenge<<'\n';
       }
     else if(i->Id()==AuftragBase::dispo_auftrag_id)
       {
         AuftragBase::mengen_t mt=i->updateStkDiffBase__(uid,-abschreibmenge);
//cerr << "Produziert:: updateStkDiffBase__ bei "<<*i<<" Menge:"<<abschreibmenge<<'\n';
         if(mt!=-abschreibmenge) fehler(instanz,Mehr_ausgelagert_als_im_Lager_war,
                                        i->Id(),abschreibmenge,-mt);
        }   
     else // plan_auftrag_id || handplan_auftrag_id
       {
        i->abschreiben(abschreibmenge,lfrsid);
//cerr << "Produziert:: Abschreiben bei "<<*i<<" Menge:"<<abschreibmenge<<'\n';
        if(abschreibmenge<0) // Zuordnung reduzieren beim abbestellen
            Reduce_Zuordnung_Add_Parent(*i,abschreibmenge);
       }
     abmenge-=abschreibmenge;
     if(!abmenge) break;
   }                                      
 return abmenge;
}



void ManuProC::st_produziert::Reduce_DispoEltern(const AufEintragBase &aeb,AuftragBase::mengen_t menge)
{
 ManuProC::Trace _t(ManuProC::Tracer::Auftrag, __FUNCTION__,"AEB="+aeb.str(),
   "Menge="+dtos(menge));

  std::list<AufEintragZu::st_reflist> L=AufEintragZu(aeb).get_Referenz_list(aeb,false);
  AuftragBase::mengen_t msumme=AuftragBase::mengen_t(0);
  for(std::list<AufEintragZu::st_reflist>::const_iterator i=L.begin();i!=L.end();++i)
   {
     AufEintrag AE(i->AEB);

     if     (AE.Id()==AuftragBase::ungeplante_id)  msumme+=i->Menge;
     else if(AE.Id()==AuftragBase::dispo_auftrag_id)
      {
        AuftragBase::mengen_t M=i->Menge;
        if(AE.getStueck()<i->Menge) M=AE.getStueck();
        AE.updateStkDiffBase__(uid,-i->Menge);
        msumme+=M;
      } 
    else assert(!"never get here\n");
    if(msumme==menge) break;
   }  
}


void ManuProC::st_produziert::Reduce_Zuordnung_Add_Parent(const AufEintragBase &aeb,AuftragBase::mengen_t menge)
{
 ManuProC::Trace _t(ManuProC::Tracer::Auftrag, __FUNCTION__,"AEB="+aeb.str(),
   "Menge="+dtos(menge));

   std::list<AufEintragZu::st_reflist> L=AufEintragZu(aeb).get_Referenz_list_ungeplant(false);
   for(std::list<AufEintragZu::st_reflist>::const_iterator i=L.begin();i!=L.end();++i)
    {
      AuftragBase::mengen_t m=AufEintragZu(i->AEB).setMengeDiff__(aeb,menge);
      i->AEB.updateStkDiffBase__(uid,-m);

      if(m==menge) break;
      menge-=m;
    }
}


void ManuProC::st_produziert::check_dispo_auftraege(ppsInstanz::ID instanz)
{
#warning Erstmal abgeschaltet. Wir müssen noch einmal genau Überdenken
#warning warum das genau gebraucht wird. Und einen entsprechenden Test
#warning dafür designen
return;

  AuftragBase::mengen_t freie_menge2=
               get_Menge_for(instanz,AuftragBase::dispo_auftrag_id);
  AuftragBase::mengen_t geplante_menge1=
               get_Menge_for(instanz,AuftragBase::AuftragBase::PlanId_for(instanz));

//cout<< "PI: "<<AuftragBase::AuftragBase::PlanId_for(instanz)
//   <<' '<<cH_ppsInstanz(instanz)->Name()
//   <<'\t'<<geplante_menge1<<' '<<freie_menge2<<'\n';
  if(geplante_menge1<freie_menge2)
   {
//cout << "Do something\n";
     abschreiben_oder_reduzieren(instanz,AuftragBase::dispo_auftrag_id,freie_menge2-geplante_menge1);
   }
}


AuftragBase::mengen_t ManuProC::st_produziert::get_Menge_for(ppsInstanz::ID instanz,int id_)
{
  SQLFullAuftragSelector sel=SQLFullAuftragSelector(SQLFullAuftragSelector::sel_Artikel_Planung_id
          (instanz,kunde,artikel,id_));
  SelectedFullAufList L(sel);
  AuftragBase::mengen_t m=0;
  for(SelectedFullAufList::iterator i=L.begin();i!=L.end();++i)
     m+=i->getStueck();
  return m;
}


void ManuProC::st_produziert::fehler(ppsInstanz::ID instanz,Probleme typ,int id,AuftragBase::mengen_t m1,
                                      AuftragBase::mengen_t m2)
{
  std::string s;
  if(typ==Mehr_produziert_als_moeglich) 
   { s="Produziert hat in "+cH_ppsInstanz(instanz)->Name()+
               " nicht die komplette Menge ("+itos(m1)+") reduzieren"
               " (bzw. abschreiben) können, sondern nur "+itos(m2);
   }
  if(typ==Mehr_ausgelagert_als_im_Lager_war) 
   { s="Produziert hat aus Lager "+cH_ppsInstanz(instanz)->Name()+
                  " mehr Menge ausgelagert ("+itos(m1)+") als drin war ("
                  +itos(m2)+")\n";
   }
  std::cerr << s<<'\n';
  AdminProblems::create("Produziert",s);
}
