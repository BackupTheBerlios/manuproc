// $Id: ppsInstanzProduziert.cc,v 1.11 2002/12/05 14:54:18 thoma Exp $
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
#include <Aux/Transaction.h>
#include <Misc/relops.h>

void ppsInstanz::Planen(ManuProC::st_produziert &P) const throw(SQLerror)
{
 ManuProC::Trace _t(ManuProC::Tracer::Auftrag, __FUNCTION__,
   "Instanz=",*this,"Artikel=",P.artikel,
   "Menge=",P.menge,"LfrsId="+itos(P.lfrsid));
 assert(Id()!=ppsInstanzID::None) ; 
 assert(!P.AE.valid()) ; 
 Transaction tr;
 if(P.menge>=0)
   {
      P.abschreiben_oder_reduzieren(Id(),
               AuftragBase::ungeplante_id,P.menge,false);
      if(P.ZNr==ManuProcEintrag::none_znr) //kein Auftrag zum Planen gefunden
       {
         // 0er erzeugen
         AuftragBase AB(this,AuftragBase::ungeplante_id);
         AB.tryUpdateEntry(0,P.lieferdatum,P.artikel,OPEN,P.uid,AufEintragBase());
         P.abschreiben_oder_reduzieren(Id(),
                  AuftragBase::ungeplante_id,P.menge,false);
       }
   }
 else //if(menge<0)
    {
      assert(!"nicht implementiert");     
    }  
  tr.commit();
}


void ppsInstanz::Produziert(ManuProC::st_produziert &P,ManuProC::Auftrag::Action reason) const throw(SQLerror)
{
 ManuProC::Trace _t(ManuProC::Tracer::Auftrag, __FUNCTION__,
   "Instanz=",*this,"Artikel=",P.artikel,
   "Menge=",P.menge,"LfrsId="+itos(P.lfrsid));
 assert(Id()!=ppsInstanzID::None) ; 
 Transaction tr;
 if(P.AE.valid()) // direktes Abschreiben ohne Suche von offenen Aufträgen
     { 
       P.AE.abschreiben(P.menge,P.lfrsid);
       P.Reduce_DispoEltern(P.AE,P.menge);      
       rekursion(P);
       tr.commit();
       return;
     }     

//cout << Name()<<'\n';

   if(P.menge>=0)
    {
      AuftragBase::mengen_t restmenge=P.abschreiben_oder_reduzieren(Id(),
                  AuftragBase::PlanId_for(Id()),P.menge);

//cout << Name()<<'\t'<<P.menge<<' '<<restmenge<<'\n';
      if(restmenge>0)
       {
          AuftragBase::mengen_t restmenge2=
             P.abschreiben_oder_reduzieren(Id(),
                     AuftragBase::ungeplante_id,restmenge);

//cout << '\t'<<Name()<<' '<<Id()<<'\t'<<P.menge<<' '<<restmenge<<' '<<restmenge2<<'\n';
            if(restmenge2>0 && reason==ManuProC::Auftrag::r_Produziert)
             {
              cH_ppsInstanz I(ppsInstanz::getProduktionsInstanz(P.artikel));
              if(I->PlanungsInstanz())
               {
                 ManuProC::st_produziert P2(P.artikel,restmenge2,P.uid);
                 I->Lager_abschreiben(P2);
               }
             }
        }       
     P.check_dispo_auftraege(Id());
    }
   else //if(menge<0)
    {
      AuftragBase::mengen_t restmenge=P.abschreiben_oder_reduzieren
                                    (Id(),AuftragBase::plan_auftrag_id,P.menge);
      if(restmenge<0) 
       {
         // da bin ich mal gespannt wann das hier fehlschlägt MAT 2.10.02
         assert(restmenge==P.menge);
         // war früher ein assert; war aber zu streng
         if(Id()!=ppsInstanzID::Kundenauftraege){ tr.commit(); return; }
         assert(P.AE.valid());         
         P.AE.abschreiben(restmenge,P.lfrsid);
       }
    }
  if(reason==ManuProC::Auftrag::r_Produziert) {tr.commit(); return;}
  rekursion(P);
  tr.commit();
}

void ppsInstanz::Lager_abschreiben(ManuProC::st_produziert &P) const 
{
 ManuProC::Trace _t(ManuProC::Tracer::Auftrag, __FUNCTION__,
   "Instanz=",*this,"Artikel=",P.artikel,
   "Menge=",P.menge,"LfrsId="+itos(P.lfrsid));

  assert(LagerInstanz()||PlanungsInstanz());
  Transaction tr;
  AuftragBase::mengen_t restmenge=P.abschreiben_oder_reduzieren(Id(),
                                       AuftragBase::plan_auftrag_id,P.menge);
  if(restmenge>0)
   {
     AuftragBase::mengen_t restmenge2=P.abschreiben_oder_reduzieren(Id(),
                  AuftragBase::dispo_auftrag_id,restmenge);
     if(restmenge2!=AuftragBase::mengen_t(0) && !PlanungsInstanz()) 
         P.fehler(*this,P.Mehr_produziert_als_moeglich,
               AuftragBase::dispo_auftrag_id,P.menge,restmenge2) ;
   }
  tr.commit();
}


void ppsInstanz::rekursion(ManuProC::st_produziert &P) const 
{
 ManuProC::Trace _t(ManuProC::Tracer::Auftrag, __FUNCTION__,
   "Instanz="+Name(),"Artikel=",P.artikel,
   "Menge=",P.menge,"LfrsId="+itos(P.lfrsid));
 ArtikelStamm as(P.artikel);
 
 if(as.BestellenBei()==ppsInstanzID::None) return;

//cout << *this<<'\t'<<EinlagernIn()<<'\t'
//<<cH_ppsInstanz(EinlagernIn())->AutomatischEinlagern()<<'\n';
//cout << *this<<'\t'<<GeplantVon()<<'\n';
 if(Id()!=ppsInstanz::getBestellInstanz(P.artikel)->Id())
   {
//cout << Name()<<'\t'<<EinlagernIn()<<' '
//<<cH_ppsInstanz(EinlagernIn())->AutomatischEinlagern()<<' '
//<<PlanungsInstanz()<<'\n';
    if(EinlagernIn()!=ppsInstanzID::None && 
       cH_ppsInstanz(EinlagernIn())->AutomatischEinlagern() &&
       !PlanungsInstanz())
     {
       LagerBase L(EinlagernIn()); 
       L.rein_ins_lager(P.artikel,P.menge,P.uid);
     }
    else
     {
        cH_ppsInstanz I=ppsInstanz::getBestellInstanz(P.artikel)->Id();
        P.kunde=ManuProC::DefaultValues::EigeneKundenId;
        P.AE=AufEintrag();
        P.lfrsid=LieferscheinBase::none_id;
//cout << "C2\t"<<I<<'\t'<<I->ProduziertSelbst()<<'\n';
        if(I->ProduziertSelbst())  return;
        I->Produziert(P);
        return;
     }
   }   
  if(GeplantVon()!=ppsInstanzID::None)
   {
      cH_ppsInstanz I(GeplantVon());
      ManuProC::st_produziert P2(P.artikel,P.menge,P.uid);
      I->Produziert(P2,ManuProC::Auftrag::r_Produziert);             
   }


   ArtikelBaum AB(P.artikel);
   for(ArtikelBaum::iterator i=AB.begin();i!=AB.end();++i)
     {
       cH_ppsInstanz I=ppsInstanz::getBestellInstanz(i->rohartikel);
       ManuProC::st_produziert sp(i->rohartikel,i->menge*P.menge,P.uid);
       if(I->ProduziertSelbst())  return;
       I->Produziert(sp);
     }
}

AuftragBase::mengen_t ManuProC::st_produziert::abschreiben_oder_reduzieren(ppsInstanz::ID instanz,int id,AuftragBase::mengen_t abmenge,bool planen_und_abschreiben_von_ungeplaneten)
{
 ManuProC::Trace _t(ManuProC::Tracer::Auftrag, __FUNCTION__,
   "Instanz=",instanz,"AuftragID=",id,
   "Menge=",abmenge);

  assert(id==AuftragBase::handplan_auftrag_id || id==AuftragBase::dispo_auftrag_id ||
         id==AuftragBase::plan_auftrag_id     || id==AuftragBase::ungeplante_id);

  SQLFullAuftragSelector sel;
  if(abmenge>=0) sel=SQLFullAuftragSelector(SQLFullAuftragSelector::sel_Artikel_Planung_id
                                                   (instanz,kunde,artikel,id));
  else sel=SQLFullAuftragSelector(SQLFullAuftragSelector::sel_Artikel_Planung_id
                                                   (instanz,kunde,artikel,id,CLOSED));

  SelectedFullAufList L(sel);

  for(SelectedFullAufList::iterator i=L.begin();i!=L.end();++i)
   {
     AuftragBase::mengen_t abschreibmenge;
     if(abmenge>=0) abschreibmenge=AuftragBase::min(abmenge,i->getRestStk());
     else abschreibmenge=-AuftragBase::min(-abmenge,i->getGeliefert());

//cerr << cH_ppsInstanz(instanz)->Name()<<' '<<i->Id()<<' '<<i->ZNr()<<"  Art."
//<<i->Artikel().Id()<<' '<<"M = "
//<<abmenge<<' '<<i->getRestStk()<<' '<<i->getGeliefert()<<'\t'<<abschreibmenge<<'\n';

     if(abschreibmenge==0 && 
       (i!=--L.end() || planen_und_abschreiben_von_ungeplaneten)) continue;

     if(i->Id()==AuftragBase::ungeplante_id)
       {
         if(i==--L.end()) abschreibmenge=abmenge ;
         AuftragBase zab(i->Instanz(),AuftragBase::plan_auftrag_id);
         if(auftragbase.valid()) zab=auftragbase;
         if(planen_und_abschreiben_von_ungeplaneten)
          {
            int znr=i->Planen(uid,abschreibmenge,zab,i->getLieferdatum(),ManuProC::Auftrag::r_Produziert);
            AufEintragBase zaeb(zab,znr);
            AufEintrag(zaeb).abschreiben(abschreibmenge,lfrsid);
//cerr << "Produziert:: Planen "<<zaeb<<" und Abschreiben bei "<<*i
//<<" Menge:"<<abschreibmenge<<" alte Menge: "<<abmenge<<'\n';
          }
         else 
          { 
            assert(lieferdatum.valid());
            if(lieferdatum > i->getLieferdatum()) continue;
            ZNr=i->Planen(uid,abschreibmenge,zab,lieferdatum,ManuProC::Auftrag::r_Planen);
          }
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
        if(abschreibmenge<0) // Zuordnung reduzieren beim abbestellen
              Reduce_Zuordnung_Add_Parent(*i,abschreibmenge);
        else  Reduce_Zuordnung_And_2er_Parent(*i,abschreibmenge);
       }
     abmenge-=abschreibmenge;
     if(!abmenge) break;
   }                                      
 return abmenge;
}



void ManuProC::st_produziert::Reduce_DispoEltern(const AufEintragBase &aeb,AuftragBase::mengen_t menge)
{
  ManuProC::Trace _t(ManuProC::Tracer::Auftrag, __FUNCTION__,"AEB=",aeb,"Menge=",menge);
  std::list<AufEintragZu::st_reflist> L=AufEintragZu(aeb).get_Referenz_list(aeb,false);
  for(std::list<AufEintragZu::st_reflist>::const_iterator i=L.begin();i!=L.end();++i)
   {
     if (i->AEB.Id()==AuftragBase::ungeplante_id)    menge-=i->Menge;
   }
  for(std::list<AufEintragZu::st_reflist>::const_iterator i=L.begin();i!=L.end();++i)
   {
     if(i->AEB.Id()==AuftragBase::dispo_auftrag_id)
      {
        AufEintrag AE(i->AEB);
        assert(i->Menge==AE.getStueck());
        AuftragBase::mengen_t M=AuftragBase::min(i->Menge,menge);
//cout << "Reduce_DispoEltern: "<<i->Menge<<' '<<menge<<'='<<M<<'\n';
        AufEintrag(i->AEB).updateStkDiffBase__(uid,-M);
        AufEintragZu(i->AEB).setMengeDiff__(aeb,-M);
        menge-=M;
      } 
   }  
}


void ManuProC::st_produziert::Reduce_Zuordnung_And_2er_Parent(const AufEintragBase &aeb,AuftragBase::mengen_t menge)
{
 ManuProC::Trace _t(ManuProC::Tracer::Auftrag, __FUNCTION__,"AEB=",aeb,
   "Menge=",menge);
 std::list<AufEintragZu::st_reflist> L=AufEintragZu(aeb).get_Referenz_list_dispo(false);
 for(std::list<AufEintragZu::st_reflist>::const_iterator i=L.begin();i!=L.end();++i)
  {
    AuftragBase::mengen_t m=AufEintragZu(i->AEB).setMengeDiff__(aeb,-menge);
    i->AEB.updateStkDiffBase__(uid,m);
    if(m==menge) break;
    menge-=m;
  }
}

void ManuProC::st_produziert::Reduce_Zuordnung_Add_Parent(const AufEintragBase &aeb,AuftragBase::mengen_t menge)
{
 ManuProC::Trace _t(ManuProC::Tracer::Auftrag, __FUNCTION__,"AEB=",aeb,
   "Menge=",menge);
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
               " nicht die komplette Menge ("+m1.String()+") reduzieren"
               " (bzw. abschreiben) können, sondern nur "+m2.String();
   }
  if(typ==Mehr_ausgelagert_als_im_Lager_war) 
   { s="Produziert hat aus Lager "+cH_ppsInstanz(instanz)->Name()+
                  " mehr Menge ausgelagert ("+m1.String()+") als drin war ("
                  +m2.String()+")\n";
   }
  std::cerr << s<<'\n';
  AdminProblems::create("Produziert",s);
}
