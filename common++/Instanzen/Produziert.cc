// $Id: Produziert.cc,v 1.5 2002/10/04 13:57:48 thoma Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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


#include "Produziert.h"
#include <Artikel/ArtikelBaum.h>
#include <Artikel/ArtikelStamm.h>
#include <Auftrag/sqlAuftragSelector.h>
//#include <Auftrag/selFullAufEntry.h>
#include <Aux/AdminProblems.h>    
#include <Lager/Lager.h>

void Produziert::NichtSelbst()
{
  if(instanz->ProduziertSelbst())  return  ;
  try{
   if(menge>=0)
    {
      AuftragBase::mengen_t restmenge=abschreiben_oder_reduzieren(
                  AuftragBase::PlanId_for(instanz),menge);
      if(restmenge>0)
       {
          /*AuftragBase::mengen_t restmenge2=*/abschreiben_oder_reduzieren(
                  AuftragBase::ungeplante_id,restmenge);
#if 0
if(restmenge2>0) 
{
cerr << "Restmenge nach Abschreiben bei geplantem Auftrag war "
<<restmenge<<"\nRestmenge nach Reduzieren des ungeplanten Auftrags ist "
<<restmenge2<<"\n   und Was machen wir nun?\n"
<<"\tVorschlag: die Restmenge HIER ins Lager einlagern\n"
<<"\tFrage: Kann die Restmenge auch bei nicht-Lager-Instanzen > 0 sein?\tJa, sie kann :-("
<< instanz->BestellungFuer()<<'\n';
//assert(cH_ppsInstanz(instanz->BestellungFuer())->LagerInstanz());

//assert (cH_ppsInstanz(instanz(instanz->BestellungFuer()))->LagerInstanz());
}
#endif
        }       
     check_dispo_auftraege();
    }
   else //if(menge<0)
    {
      AuftragBase::mengen_t restmenge=abschreiben_oder_reduzieren(AuftragBase::plan_auftrag_id,menge);
      if(restmenge<0) 
       {
         // da bin ich mal gespannt wann das hier fehlschlägt MAT 2.10.02
         assert(restmenge==menge);
         assert(instanz==ppsInstanzID::Kundenauftraege) ;
         assert(AE.valid());         
         AE.abschreiben(restmenge);
       }
    }
  rekursion();
  }catch(SQLerror &e) { cerr << e<<'\n';}
}

void Produziert::Lager_abschreiben()
{
  assert(instanz->LagerInstanz());
  AuftragBase::mengen_t restmenge=abschreiben_oder_reduzieren(
                                       AuftragBase::plan_auftrag_id,menge);
  if(restmenge>0)
   {
     AuftragBase::mengen_t restmenge2=abschreiben_oder_reduzieren(
                  AuftragBase::dispo_auftrag_id,restmenge);
     if(restmenge2!=AuftragBase::mengen_t(0)) fehler(Mehr_produziert_als_moeglich,
               AuftragBase::dispo_auftrag_id,menge,restmenge2) ;
   }
}


void Produziert::rekursion()
{
 ArtikelStamm as(artikel);
 
 if(as.BestellenBei()==ppsInstanzID::None) return;

  if(instanz!=as.BestellenBei()) // LagerFür Instanzen und Kundeninstanzen
   {
     cH_ppsInstanz I=as.BestellenBei();
     Produziert(I,artikel,menge,uid).NichtSelbst();
     return;
   }   
   ArtikelBaum AB(artikel);
//cout << "Rekursion für "<<instanz->Name()<<'\n';
   for(ArtikelBaum::const_iterator i=AB.begin();i!=AB.end();++i)
     {
       ArtikelStamm AS(i->rohartikel);
//cout << "\t"<<AS.BestellenBei()->Name()<<'\n';
     }
//cout <<'\n';
   for(ArtikelBaum::const_iterator i=AB.begin();i!=AB.end();++i)
     {
       ArtikelStamm AS(i->rohartikel);
       Produziert(AS.BestellenBei(),i->rohartikel,double(i->menge)*menge,uid).NichtSelbst();
     }
}


AuftragBase::mengen_t Produziert::abschreiben_oder_reduzieren(int id,AuftragBase::mengen_t abmenge)
{

  assert(id==AuftragBase::handplan_auftrag_id || id==AuftragBase::dispo_auftrag_id ||
         id==AuftragBase::plan_auftrag_id     || id==AuftragBase::ungeplante_id);

  SQLFullAuftragSelector sel;
  if(abmenge>=0) sel=SQLFullAuftragSelector(SQLFullAuftragSelector::sel_Artikel_Planung_id
                                                   (instanz->Id(),kunde,artikel,id));
  else sel=SQLFullAuftragSelector(SQLFullAuftragSelector::sel_Artikel_Planung_id
                                                   (instanz->Id(),kunde,artikel,id,CLOSED));

  SelectedFullAufList L(sel);

//cout <<"SIZE: "<< L.size()<<'\n';
  for(SelectedFullAufList::iterator i=L.begin();i!=L.end();++i)
   {
     AuftragBase::mengen_t abschreibmenge;
     if(i->getRestStk() >= abmenge) abschreibmenge = abmenge;
     else                           abschreibmenge = i->getRestStk();

//cout << instanz->Name()<<' '<<i->Id()<<' '<<i->ZNr()<<"  Art."
//<<i->Artikel().Id()<<' '<<"M = "
//<<abmenge<<' '<<i->getRestStk()<<'\t'<<abschreibmenge<<'\n';

     if(abschreibmenge==AuftragBase::mengen_t(0)) continue;     

     if(i->Id()==AuftragBase::ungeplante_id)
       {
         AuftragBase zab(i->Instanz(),AuftragBase::plan_auftrag_id);
         int znr=i->Planen(uid,abschreibmenge,true,zab,i->getLieferdatum());
         AufEintragBase zaeb(zab,znr);
         AufEintrag(zaeb).abschreiben(abschreibmenge,lfrsid);
//cout << "Produziert:: Planen "<<zaeb<<" und Abschreiben bei "<<*i<<" Menge:"<<abschreibmenge<<'\n';
       }
     else if(i->Id()==AuftragBase::dispo_auftrag_id)
       {
         AuftragBase::mengen_t mt=i->updateStkDiffBase__(uid,-abschreibmenge);
//cout << "Produziert:: updateStkDiffBase__ bei "<<*i<<" Menge:"<<abschreibmenge<<'\n';
         if(mt!=-abschreibmenge) fehler(Mehr_ausgelagert_als_im_Lager_war,
                                        i->Id(),abschreibmenge,-mt);
        }   
     else // plan_auftrag_id || handplan_auftrag_id
       {
        i->abschreiben(abschreibmenge,lfrsid);
//cout << "Produziert:: Abschreiben bei "<<*i<<" Menge:"<<abschreibmenge<<'\n';
        if(abschreibmenge<0) // Zuordnung reduzieren beim abbestellen
            Reduce_Zuordnung_Add_Parent(*i,abschreibmenge);
       }
     abmenge-=abschreibmenge;
     if(!abmenge) break;
   }                                      
 return abmenge;
}

void Produziert::Reduce_Zuordnung_Add_Parent(const AufEintragBase &aeb,AuftragBase::mengen_t menge)
{
   std::list<AufEintragZu::st_reflist> L=AufEintragZu(aeb).get_Referenz_list_ungeplant(false);
   for(std::list<AufEintragZu::st_reflist>::const_iterator i=L.begin();i!=L.end();++i)
    {
      AuftragBase::mengen_t m=AufEintragZu(i->AEB).setMengeDiff__(aeb,menge);
      i->AEB.updateStkDiffBase__(uid,-m);

      if(m==menge) break;
      menge-=m;
    }
}


void Produziert::check_dispo_auftraege()
{
  AuftragBase::mengen_t freie_menge2=
               get_Menge_for(AuftragBase::dispo_auftrag_id);
  AuftragBase::mengen_t geplante_menge1=
               get_Menge_for(AuftragBase::AuftragBase::PlanId_for(instanz));

//cout<< "PI: "<<instanz->Name()<<'\t'<<geplante_menge1<<' '<<freie_menge2<<'\n';
  if(geplante_menge1<freie_menge2)
   {
//cout << "Do something\n";
     abschreiben_oder_reduzieren(AuftragBase::dispo_auftrag_id,freie_menge2-geplante_menge1);
   }
}

AuftragBase::mengen_t Produziert::get_Menge_for(int id_)
{
  SQLFullAuftragSelector sel=SQLFullAuftragSelector(SQLFullAuftragSelector::sel_Artikel_Planung_id
          (instanz->Id(),kunde,artikel,id_));
  SelectedFullAufList L(sel);
  AuftragBase::mengen_t m=0;
  for(SelectedFullAufList::iterator i=L.begin();i!=L.end();++i)
     m+=i->getStueck();
  return m;
}



void Produziert::fehler(Probleme typ,int id,AuftragBase::mengen_t m1,
                                      AuftragBase::mengen_t m2)
{
  std::string s;
  if(typ==Mehr_produziert_als_moeglich) 
   { s="Produziert hat in "+instanz->Name()+
               " nicht die komplette Menge ("+itos(m1)+") reduzieren"
               " (bzw. abschreiben) können, sondern nur "+itos(m2);
   }
  if(typ==Mehr_ausgelagert_als_im_Lager_war) 
   { s="Produziert hat aus Lager "+instanz->Name()+
                  " mehr Menge ausgelagert ("+itos(m1)+") als drin war ("
                  +itos(m2)+")\n";
   }
  cerr << s<<'\n';
  AdminProblems::create("Produziert",s);
}
