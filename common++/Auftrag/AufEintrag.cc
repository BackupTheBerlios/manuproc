// $Id: AufEintrag.cc,v 1.26 2003/01/08 17:40:43 christof Exp $
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

#include"AufEintrag.h"
#include <Misc/string0.h>
#include <Misc/Transaction.h>
#include <Auftrag/AufEintragZu.h>
#include <Auftrag/Auftrag.h>
#include <unistd.h>
#include <Instanzen/ppsInstanzProduziert.h>
#include <Misc/Trace.h>
#include <Lager/Lager.h>
#include <Auftrag/AufEintragZuMengenAenderung.h>

AufEintrag::AufEintrag(const AufEintragBase &aeb, mengen_t _bestellt,
	ArtikelBase _artikel, const ManuProC::Datum _lieferdatum,
	AufStatVal _aufstatus, int _kdnr, const std::string _youraufnr,
	const Preis &_preis, rabatt_t _rabatt, AufStatVal _entrystat, 
	int uid, const cH_PreisListe &_preisliste) throw()
: AufEintragBase(aeb),
 bestellt(_bestellt),
 geliefert(0),
 artikel(_artikel),
 entrystatus(_entrystat),
 lieferdatum(_lieferdatum),
 lasteditdate_uid(uid),
 lasteditdate(ManuProC::Datum::today()),
 letztePlanInstanz(ppsInstanzID::None),
 maxPlanInstanz(0),
 preis(_preis),
 rabatt(_rabatt),
 kdnr(_kdnr),
 youraufnr(_youraufnr),
 disponr(0),
 auftragstatus(_aufstatus), 
 dispoentrynr(0),
 prozess(Prozess::default_id),
 preisliste(_preisliste)
{
}
	
AufEintrag::AufEintrag(ppsInstanz::ID _instanz,int _auftragid, int _zeilennr,
   mengen_t _bestellt,
	ArtikelBase _artikel, const ManuProC::Datum _lieferdatum,
	AufStatVal _entrystatus) throw()
: AufEintragBase(_instanz,_auftragid,_zeilennr),
 bestellt(_bestellt),
 geliefert(0),
 artikel(_artikel),
 entrystatus(_entrystatus),
 lieferdatum(_lieferdatum),
 lasteditdate_uid(0),
 auftragstatus(_entrystatus),
 prozess(Prozess::default_id)
{}
	

const Preis AufEintrag::GPreis() const
{ return preis.Gesamtpreis(1,bestellt.as_float(),rabatt);
}

const Preis AufEintrag::EPreis(bool brutto) const
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__);
 if(brutto) return preis;
 else
 return preis.Gesamtpreis(preis.PreisMenge().as_int(),0,rabatt);
}


void AufEintrag::setLetztePlanungFuer(cH_ppsInstanz planinstanz) throw(SQLerror)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__);
  AufEintragBase::setLetztePlanungFuer(planinstanz);
  letztePlanInstanz=planinstanz->Id();
}


#if 0
void AufEintrag::setVerarbeitung(const cH_Prozess p)
{
 AufEintragBase::setVerarbeitung(p);

 prozess=p;
 prozdate=ManuProC::Datum().today();
}
#endif


void AufEintrag::setLetzteLieferung(const ManuProC::Datum &datum) throw(SQLerror)
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__);
 AufEintragBase::setLetzteLieferung(datum);
 letzte_lieferung=datum;
}


const std::string AufEintrag::getEntryStatusStr() const
{  return AuftragBase::getStatusStr(entrystatus);
}

bool AufEintrag::allesOK() const
{
 if (!getStueck()) return false;
 if (!getLieferdatum().valid()) return false;
 return true;
}

std::string AufEintrag::Planung() const
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__);
  int tiefe = ArtikelBaum(ArtId()).Tiefe();
  return itos(maxPlanInstanz)+"/"+itos(tiefe);  
}



#if 0
void AufEintrag::move_to(int uid,AufEintragBase AEB,AuftragBase::mengen_t menge,ManuProC::Auftrag::Action reason) throw(std::exception)
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this,"To=",AEB,"Menge=",menge,"Reason=",reason);
  Transaction tr;

  if(reason==ManuProC::Auftrag::r_Produziert || 
     reason==ManuProC::Auftrag::r_Planen || 
     reason==ManuProC::Auftrag::r_Closed  ||
     reason==ManuProC::Auftrag::r_Reparatur)
   {
     mengen_t mt1=updateStkDiff__(uid,-menge,false,reason);
     assert(-menge==mt1);
   }
  mengen_t mt2=AufEintrag(AEB).updateStkDiff__(uid,menge,false);
  assert(menge==mt2);
  AufEintragZu(*this).Neu(AEB,menge); 
  tr.commit();
}      
#endif

AufEintragBase AufEintrag::getFirstKundenAuftrag() const
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__);
 std::vector<AufEintragBase> V=AufEintragZu(*this).getKundenAuftragV();
 if(V.empty()) return *this;
 else return *(V.begin());
}



#if 0
void AufEintrag::move_menge_to_dispo_zuordnung_or_lager(mengen_t menge,const ArtikelBase artikel,int uid,ManuProC::Auftrag::Action reason)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,"AE=",*this,"Menge=",menge,"Reason=",reason);
 std::list<AufEintragZu::st_reflist> K=AufEintragZu(*this).get_Referenz_list_geplant();
 for (std::list<AufEintragZu::st_reflist>::const_iterator i=K.begin();i!=K.end();++i)
  {
   if(artikel!=i->Art) continue;
   AuftragBase::mengen_t M=AuftragBase::min(i->Menge,menge);
   AufEintragZu(*this).setMengeDiff__(i->AEB,-M);
   if(i->AEB.Instanz()->LagerInstanz())
     {
      AufEintrag AE(i->AEB);
      mengen_t mt=i->AEB.updateStkDiffBase__(uid,-M);
      dispo_auftrag_aendern(uid,AE.Instanz(),AE.Artikel(),M);
      menge_neu_verplanen(uid,AE.Instanz(),AE.Artikel(),M,reason);
      assert(mt==mengen_t(-M));
     }
  }
/*
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
      dispo_auftrag_aendern(uid,Instanz(),Artikel(),M);
      menge_neu_verplanen(uid,Instanz(),Artikel(),M,reason);
      assert(mt==mengen_t(-M));
     }
    else
     {
      std::list<AufEintragZu::st_reflist> E=AufEintragZu(i->AEB).get_Referenz_list_dispo(false);
      if(E.empty())
       {
         Auftrag ab((AuftragBase(Instanz(),dispo_auftrag_id)));
         AufEintragBase newaeb = ab.push_back(0,getLieferdatum(),Artikel(),getEntryStatus(),uid,false); 
         E.push_back(AufEintragZu::st_reflist(newaeb,Artikel(),0));
       }
      AufEintragBase aep_dispo;
      for (std::list<AufEintragZu::st_reflist>::const_iterator j=E.begin();j!=E.end();++j)
        {
         AufEintragZu(j->AEB).Neu(i->AEB,M); // versucht erst ein 'update'
         j->AEB.updateStkDiffBase__(uid,M);
         if(j->AEB.Id()==dispo_auftrag_id) aep_dispo=j->AEB;
        }
      std::list<AufEintragZu::st_reflist> R=AufEintragZu(*this).get_Referenz_list(*this,true);
      for(std::list<AufEintragZu::st_reflist>::const_iterator j=R.begin();j!=R.end();++j)
       {
         if(reason==ManuProC::Auftrag::r_Closed && j->AEB.Id()==ungeplante_id)
            AufEintragZu(aep_dispo).Neu(j->AEB,j->Menge);
       }
     }
    menge-=M;
    if(menge==AuftragBase::mengen_t(0)) return;
  }
*/
}
#endif

void AufEintrag::Produziert(mengen_t menge,
   ManuProcEntity<>::ID lfrsid) throw(SQLerror)
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,"Menge=",menge);
  Kunde::ID kunde=Auftrag(*this).getKundennr();
  ManuProC::st_produziert sp(kunde,*this,menge,getuid(),lfrsid);
  Instanz()->Produziert(sp,ManuProC::Auftrag::r_Produziert);
}


int AufEintrag::Planen(int uid,mengen_t menge,const AuftragBase &zielauftrag,
   const Petig::Datum &datum,ManuProC::Auftrag::Action reason,
   AufEintragBase *verplanter_aeb,bool rekursiv) throw(std::exception)
{
   Transaction tr;
   if(verplanter_aeb) *verplanter_aeb=*this;
   ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,"Menge=",menge,"Reason=",reason,"Zielauftrag=",zielauftrag,"rekursiv=",rekursiv);
   assert(Id()==AuftragBase::ungeplante_id);
// wieso dürfen LagerAufträge geschlossen sein?
   assert(/*Instanz()->LagerInstanz() || */entrystatus==OPEN);
   assert(/*Instanz()->LagerInstanz() || */auftragstatus==OPEN);
// gefällt mir nicht CP   
//   if(entrystatus==UNCOMMITED) entrystatus=OPEN; // Kein Datenbankzugriff nötig, 
                                                // das macht insertNewEntry
   Auftrag ZA(zielauftrag);
   ZA.setStatusAuftrag_(OPEN);

/*
   // wenn Lieferdatum rekursiv korrigieren (VOR dem Planen)
   if(datum!=getLieferdatum())
    {
      AuftragBase::mengen_t M=AuftragBase::min(menge,getStueck());
      int znr=split(uid,M,datum);
      AufEintrag ae=AufEintrag(AufEintragBase(*this,znr));
      tr.commit();
      return ae.Planen(uid,menge,zielauftrag,datum,reason,verplanter_aeb,rekursiv);
    }
*/

   int znr=ManuProcEntity<>::none_id,dummy=ManuProcEntity<>::none_id;
   mengen_t mdummy=0;

// wenn schon eine Zeile mit diesem Datum/Artikel/Status in diesem Auftrag
// existiert, Menge erhöhen. So kann man von Aufträge aus verschiedenen 
// Quellen zusammen fertigen
   if (!zielauftrag.existEntry(Artikel(),datum,znr,dummy,mdummy,entrystatus))
    {
      AufEintragBase neuAEB=ZA.push_back(0,datum,Artikel(),entrystatus,uid,false);
      znr=neuAEB.ZNr();
    }

// nix zu planen -> erledigt   
   if(menge==0) {tr.commit();   return znr;}
   assert(menge>0);
   
   // dispo (2er) Auftrag anlegen bei Überplanung
   if (menge-getRestStk() > 0) //  && !Instanz()->LagerInstanz() )
    { assert(!Instanz()->LagerInstanz()); // CP
      mengen_t dispomenge = menge-getRestStk();
      // nur soviel unten verwenden (tatsächlich zu uns ziehen, 
      // 	Rest wird von uns in Ueberplanen bestellt)
      menge=getRestStk();
      
      AufEintragBase AEB(zielauftrag,znr);
      AufEintrag(AEB).Ueberplanen(uid,Artikel(),dispomenge,datum);

      // Produktionsplaner (ungetestet)
      if(zielauftrag.Instanz()->GeplantVon()!=ppsInstanzID::None)
       {
	    assert(!"yet implemented");
#if 0	    
         if(reason==ManuProC::Auftrag::r_Planen) 
           {
            AuftragBase DAB(zielauftrag.Instanz()->GeplantVon(),dispo_auftrag_id);
            int znr=DAB.tryUpdateEntry(dispomenge,datum,Artikel(),OPEN,uid,AufEintragBase());
            AuftragBase ab(Instanz(),ungeplante_id);
            int nzr=ab.tryUpdateEntry(-dispomenge,datum,Artikel(),OPEN,uid,AufEintragBase());
/*
            AufEintragZuMengenAenderung::move_zuordnung_zu_geplantem(AufEintragBase(DAB,znr),
                                                                     AufEintragBase(ab,nzr),
                                                                     dispomenge);       
*/
            AufEintragZu((class AufEintragBase(DAB,znr))).Neu(AufEintragBase(ab,nzr),dispomenge);
           }
         else
           {
//cout << "ELSEn";
           }
#endif            
       }
    }
  AufEintrag AE1er(AufEintragBase(zielauftrag,znr));
  AufEintragZuMengenAenderung::move_zuordnung_zu_geplantem(uid,*this,
                                                           AE1er,menge,reason);

// welche reasons sollte es sonst geben? CP
  if (reason!=ManuProC::Auftrag::r_Planen)
     std::cerr << "??? Aha, Planen und doch nicht planen? CP\n";
  
  if(reason==ManuProC::Auftrag::r_Planen) 
   {
     updateStkDiff__(uid,-menge,true,AufEintragBase(),reason);
     
     // warum !=plan_auftrag_id ??? CP
     if(!Instanz()->LagerInstanz() && Id()!=AuftragBase::plan_auftrag_id)
        InstanzAuftraegeAnlegen(AE1er,menge,uid);
   }
//  else
//     AE1er.updateStkDiff__(uid,menge,true,AufEintragBase(),reason);
     AE1er.updateStkDiff__(uid,menge,false,AufEintragBase(),reason);

//   move_to(uid,AufEintragBase(zielauftrag,znr),menge,reason);

  // nur wenn aktiv durch Benutzer geplant 
  if (zielauftrag.Id() != AuftragBase::plan_auftrag_id)
  { assert(zielauftrag.Id()>2);
   std::list<AufEintragZu::st_reflist> ReferenzAufEintrag =
			         AufEintragZu(*this).get_Referenz_listFull(false);
   for (std::list<AufEintragZu::st_reflist>::iterator i=ReferenzAufEintrag.begin();i!=ReferenzAufEintrag.end();++i)
    {
     if(i->AEB.Instanz()->Id()!=ppsInstanzID::Kundenauftraege) continue;
     if(zielauftrag.Id()==AuftragBase::plan_auftrag_id) continue;
     i->AEB.setLetztePlanungFuer(instanz);
     i->AEB.calculateProzessInstanz();
    }
  }

  assert(!rekursiv);
 tr.commit();
 return znr;
}

void AufEintrag::ProduktionsPlanung(int uid,mengen_t menge,const AuftragBase &zielauftrag, 
      const ManuProC::Datum &datum,cH_ppsInstanz instanz) throw(std::exception)
{
//   Transaction tr;
   assert(Id()==AuftragBase::ungeplante_id);
   AufEintragBase newAEB;
   Planen(uid,menge,zielauftrag,datum,ManuProC::Auftrag::r_Planen,&newAEB);
   AuftragBase zielauftrag_instanz(instanz,AuftragBase::ungeplante_id);
   int znr=zielauftrag_instanz.tryUpdateEntry(menge,datum,Artikel(),OPEN,uid,newAEB);
//   tr.commit();
}


#if 0
void AufEintrag::menge_fuer_aeb_freigeben(const mengen_t &menge,AufEintrag &ae,const int uid)
{
  Transaction tr;  
  AuftragBase::mengen_t M=AuftragBase::min(getRestStk(),menge);
  if(!M) return;

  // Alte Reservierung aufheben
  std::list<AufEintragZu::st_reflist> L=AufEintragZu(*this).get_Referenz_list_ungeplant(false);
  for(std::list<AufEintragZu::st_reflist>::const_iterator i=L.begin();i!=L.end();++i)
   {
    AufEintrag(i->AEB).updateStkDiff__(uid,M,true,ManuProC::Auftrag::r_Anlegen);
    AufEintragZu(i->AEB).setMengeDiff__(*this,-M);
   }
  updateStkDiffBase__(uid,-M);
  //neue Reservierung anlegen
  ae.updateStkDiff__(uid,-M,true,ManuProC::Auftrag::r_Anlegen);
  AuftragBase AB(Instanz(),plan_auftrag_id);
  AuftragBase::st_tryUpdateEntry st(false,false,true);
  int znr=AB.tryUpdateEntry(M,ae.getLieferdatum(),ae.Artikel(),OPEN,uid,AufEintragBase(),st);  
  AufEintragZu(ae).Neu(AufEintragBase(AB,znr),M);

 tr.commit();
}
#endif

// einen Dispo Auftrag für einen Auftrag anlegen (wegen freier Menge)
void AufEintrag::Ueberplanen(int uid,const ArtikelBase& artikel,mengen_t menge,const ManuProC::Datum &datum)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,str(),
   "Artikel=",artikel,"Menge=",menge,"Datum=",datum);
   // automatisch geplant ??
   assert(Id()!=plan_auftrag_id);

   AuftragBase dispoAB(Instanz(),dispo_auftrag_id);
   st_tryUpdateEntry st(false,false,true);
   //int znr=
   dispoAB.tryUpdateEntry(menge,datum,artikel,OPEN,uid,*this,st);

   // zusätzliche Menge vermerken und Material bestellen
   updateStkDiff__(uid,menge,true,AufEintragBase(),ManuProC::Auftrag::r_Anlegen); // oder Planen?
//   updateStkDiffBase__(uid,menge);
//   InstanzAuftraegeAnlegen(*this,menge,uid); // ,(Id()==plan_auftrag_id));
}
