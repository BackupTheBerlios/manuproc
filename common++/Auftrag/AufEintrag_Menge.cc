// $Id: AufEintrag_Menge.cc,v 1.16 2003/09/15 12:19:26 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2003 Adolf Petig GmbH & Co. KG
 *  written by Jacek Jakubowski & Christof Petig
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

#include <Auftrag/AufEintrag.h>
#include <Misc/FetchIStream.h>
#include <Misc/string0.h>
#include <Misc/Transaction.h>
#include <Auftrag/AufEintragZu.h>
#include <Auftrag/Auftrag.h>
#include <unistd.h>
#include <Misc/TraceNV.h>
#include <Lager/Lager.h>
#include <Auftrag/VerfuegbareMenge.h>
#include <Misc/Changejournal.h>
//#include <Lieferschein/Lieferschein.h>
#include <Auftrag/AufEintrag_loops.h>
#include <Misc/inbetween.h>
#include <Artikel/ArtikelStamm.h>
#include <Misc/relops.h>
#include <Auftrag/sqlAuftragSelector.h>
#include <Auftrag/selFullAufEntry.h>
#include <Auftrag/AufEintrag_delayedreclaim.h>

#ifdef MABELLA_EXTENSIONS
//#include <Lager/FertigWarenLager.h>
#include <Artikel/ArtikelBase.h>
#endif

// statische Variante (behandelt z.B. Vormerkungen, freie Menge etc.)
AufEintragBase AufEintrag::ArtikelInternNachbestellen(const cH_ppsInstanz &wo,
  mengen_t menge,const ManuProC::Datum &lieferdatum,const ArtikelBase& artikel,
  const AufEintragBase& ElternAEB)
{  ManuProC::Trace _t(trace_channel, __FUNCTION__,
   	wo,menge,lieferdatum,artikel,NV("ElternAEB",ElternAEB));
   if (!menge) return AufEintragBase(); // hmmm
   if (wo==ppsInstanzID::None || wo==ppsInstanzID::Kundenauftraege)
   {  std::cerr << "ArtikelInternNachbestellen: ungültige Instanz für " << menge
   		<< " von " << cH_ArtikelBezeichnung(artikel)->Bezeichnung()
   		<< " Elter " << ElternAEB << '\n';
      return AufEintragBase();
   }
   assert(menge>0);
   assert(ElternAEB.valid());

   // 2er Menge (freie Menge im Lager, freie Produktionsplanung)
   // vormerken
   VerfuegbareMenge AIL(wo,artikel,lieferdatum);
   mengen_t M_dispo=min(AIL.getMengeDispo(),menge);
   if(M_dispo>0) menge-=AIL.reduce_in_dispo(M_dispo,ElternAEB);

   // Im Lager von späteren Aufträgen vorgemerkte Menge wegschnappen
   if (menge>0 && wo->LagerInstanz())
   {  M_dispo=min(AIL.getMengePlan(),menge);
      menge-=AIL.reduce_in_plan(M_dispo,ElternAEB);
   }
   // Rest nachbestellen
   AuftragBase ab(wo,ungeplante_id);
   int znr=none_znr;
   if (menge>0)
      znr=ab.BestellmengeAendern(menge,lieferdatum,artikel,OPEN,ElternAEB);
   return AufEintragBase(ab,znr);
}

// ehemals updateStkDiffInstanz__ mit menge>0
// + ehemals BaumAnlegen
void AufEintrag::ArtikelInternNachbestellen(mengen_t menge) const
{
  ManuProC::Trace _t(trace_channel, __FUNCTION__,*this,
   NV("menge",menge));
  assert(menge>0);
   // Rekursion von 2ern verbieten
   assert(Id()!=dispo_auftrag_id);

   // 1er im Lager wird erhöht ... nichts tun
   if (Instanz()->LagerInstanz() && Id()==plan_auftrag_id)
      return;
  ppsInstanz::ID next=Instanz()->NaechsteInstanz(ArtikelStamm(Artikel()));
  if (next!=ppsInstanzID::None)
  {  AufEintrag::ArtikelInternNachbestellen(next,menge,getLieferdatum(),
  			Artikel(),*this);
  }
  else
  {  ManuProC::Datum newdate=getLieferdatum()-Instanz()->ProduktionsDauer();

     ArtikelBaum AB(Artikel());
     for(ArtikelBaum::const_iterator i=AB.begin();i!=AB.end();++i)
     {  ArtikelInternNachbestellen(ppsInstanz::getBestellInstanz(i->rohartikel),
  		i->menge*menge,newdate,i->rohartikel,*this);
     }
  }
}

// bei 2ern geht der Pfeil in die andere Richtung
AuftragBase::mengen_t AufEintrag::MengeAendern(mengen_t menge,bool instanzen,
     const AufEintragBase &ElternAEB) throw(SQLerror)
{
 ManuProC::Trace _t(trace_channel, __FUNCTION__,*this,
   NV("Eltern",ElternAEB),
   NV("menge",menge),NV("instanzen",instanzen));
 if (!menge)  return menge;

 Transaction tr; // Beschleunigung
 Query("lock auftragentry in exclusive mode");
 SQLerror::test("updateStkDiff: lock table auftragentry");

 mengen_t menge2=menge;
 if (menge2<0) // nicht mehr abbestellen als noch offen ist
 		// aber dennoch den Pfeil reduzieren (wegen ProdSelbst)
 {  menge2=-min(-menge2,getRestStk());
 }
 if (!!menge2)
 { menge2=updateStkDiffBase__(menge2);
   bestellt+=menge2;
   if (menge2>0 && entrystatus==CLOSED)
      setStatus(OPEN,true);
   else if (Id()!=ungeplante_id && Id()!=dispo_auftrag_id 
   		&& !getRestStk() && entrystatus==OPEN
   		&& (!Instanz()->LagerInstanz() || !!bestellt))
      setStatus(CLOSED);
 }

 if (ElternAEB.valid())
 {  if (Id()!=dispo_auftrag_id) AufEintragZu(ElternAEB).Neu(*this,menge);
    // bei dispoaufträgen geht der Pfeil in die andere Richtung
    else AufEintragZu(*this).Neu(ElternAEB,menge);
 }

 if(auftragstatus==OPEN && instanzen && !!menge2)
  {
   // Rekursion von 0ern im Lager (es gibt keine 3er im Lager)
   //  Verplanen von freigewordener Menge bei 1er im Lager
   // Rekursion bei 0er, 1er oder 3er in Produktion
      updateStkDiffInstanz__(menge2);
  }
  tr.commit();
  // wir haben zwar weniger abbestellt, aber nur weil wir geliefert haben
 return menge;
}

AuftragBase::mengen_t AufEintrag::ArtikelInternAbbestellen_cb::operator()
	(const ArtikelBase &i,const AufEintragBase &j,mengen_t M) const
{  AufEintrag AE(j);
   if (j.Id()==ungeplante_id)
      M=-AE.MengeAendern(-M,true,mythis);
   else if (j.Instanz()->LagerInstanz())
   {  // vorgemerkte Menge freigeben
      assert(j.Id()==plan_auftrag_id);
      M=-AE.MengeAendern(-M,true,mythis);
   }
   else
     { // [1er oder] 3er - dispo anlegen, Bestellpfeil erniedrigen
       assert(j.Id()>=handplan_auftrag_id);
       mengen_t noch_frei=AuftragBase::min(M,AE.getRestStk());
#warning geht bei EinlagernIn+Automatisch einlagern von Teilmenge fehl?       
       if (!!noch_frei)
       {  AuftragBase(j.Instanz(),dispo_auftrag_id)
       	     .BestellmengeAendern(noch_frei,AE.getLieferdatum(),AE.Artikel(),OPEN,j);
       }
       AufEintragZu(mythis).setMengeDiff__(j,-M);
     }
   return M;
}

class AufEintrag::ArtikelAbbestellen_bevorzugt : public distribute_children_twice_cb
{	ArtikelInternAbbestellen_cb backend;

public:
	ArtikelAbbestellen_bevorzugt(const AufEintrag &_mythis)
		: backend(_mythis)
	{}
	// falls LagerInstanz 1er zuerst nehmen
	mengen_t operator()(const ArtikelBase &art,
 		const AufEintragBase &aeb,AuftragBase::mengen_t m,bool first) const
 	{  if (aeb.Instanz()->LagerInstanz() && aeb.Id()==handplan_auftrag_id)
 	   {  if (first) return backend(art,aeb,m);
 	   }
 	   else
 	   {  if (!first) return backend(art,aeb,m);
 	   }
 	   return 0;
 	}
 	void operator()(const ArtikelBase &art,AuftragBase::mengen_t m) const 
 	{  backend(art,m); }
};

void AufEintrag::ArtikelInternAbbestellen(mengen_t menge) const
{ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("this",*this),
   NV("menge",menge));

 assert(menge>0);
 // Rekursion von 2ern verbieten
 assert(Id()!=dispo_auftrag_id);

 // Menge im Lager freigeben == Einlagern ohne Produktion?
 if (Instanz()->LagerInstanz() && Id()==plan_auftrag_id)
 {  if (delayed_reclaim::Active())
    {  // in 2er rein
       AuftragBase(make_value(Instanz()),dispo_auftrag_id)
       	.BestellmengeAendern(menge,LagerBase::Lagerdatum(),Artikel(),
       			OPEN,AufEintragBase());
       delayed_reclaim::delay(Instanz(),Artikel());
    }
    else
       AufEintrag::MengeVormerken(Instanz(),Artikel(),menge,true,ProductionContext()); 
	 // ,*this);
 }
 else 
 try{
    if (lager_bevorzugt_freigeben)
      distribute_children_twice_rev(*this,menge,Artikel(),
      			ArtikelAbbestellen_bevorzugt(*this));
    else
      distribute_children(*this,menge,Artikel(),
   			ArtikelInternAbbestellen_cb(*this));
  }catch(NoAEB_Error &e)
  {std::cerr <<"Falsche Anzahl von AufEinträgen: "<<e.what()<<'\n';
   exit(1);}
}

void AufEintrag::updateStkDiffInstanz__(mengen_t menge) throw(SQLerror)
{
 ManuProC::Trace _t(trace_channel, __FUNCTION__,
   NV("this",*this),NV("menge",menge));
 if (menge<0)
 {  ArtikelInternAbbestellen(-menge);
 }
 else
 {  ArtikelInternNachbestellen(menge);
 }
}

// angepasste Variante von ppsInstanzReparatur::Eltern
AuftragBase::mengen_t AufEintrag::AnElternMengeAnpassen()
{  ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("this",*this));
   AufEintragZu::list_t eltern=AufEintragZu::get_Referenz_list(*this,
		AufEintragZu::list_eltern,AufEintragZu::list_ohneArtikel);
   assert(Id()!=dispo_auftrag_id);
   assert(Instanz()!=ppsInstanzID::Kundenauftraege);
   mengen_t menge,menge2;
   for (AufEintragZu::list_t::iterator i=eltern.begin();i!=eltern.end();++i)
   {  menge+=i->Menge;
      if (i->AEB.Id()==dispo_auftrag_id) menge2+=i->Menge;
   }
   if (menge>getStueck())
   // mehr v.o. benötigt als jemals bestellt (z.B. durch ProdRückgängig)
   {  ManuProC::Trace(trace_channel,"(menge>getStueck())",menge,getStueck());
      if (Id()==plan_auftrag_id) // && Instanz()->LagerInstanz())
      {  // sonst ist der Code ungetestet, könnte aber funktionieren
         // bitte Nachricht an mich (CP), sobald das stört
         assert(Instanz()->LagerInstanz());
         return menge-getStueck();
      }
      assert(Id()==ungeplante_id); // sonst Problem !
      MengeAendern(menge-getStueck(),true,AufEintragBase());
   }
   else if (menge<getRestStk())
   // mehr offen als nun v.o. benötigt
   {  ManuProC::Trace(trace_channel,"(menge<getRestStk())",menge,getRestStk());
      assert(in(Id(),ungeplante_id,plan_auftrag_id));
      assert(Id()!=plan_auftrag_id || Instanz()->LagerInstanz());
      MengeAendern(menge-getRestStk(),true,AufEintragBase());
   }
   return 0;
}

// auf noch offene Menge beschränken
// auf f.o. umstellen geht nicht sinnvoll, da die Funktion ein Unikat ist
void AufEintrag::DispoBeschraenken()
{
  ManuProC::Trace _t(trace_channel, __FUNCTION__,*this);
  AufEintragZu::list_t L=AufEintragZu::get_Referenz_list(*this,AufEintragZu::list_eltern,AufEintragZu::list_ohneArtikel);

  mengen_t S=AufEintragZu::Summe(L,dispo_auftrag_id);
  if (getRestStk()<S)
  {   ManuProC::Trace(trace_channel, __FILELINE__,
  		"Dispo muss um ",S-getRestStk()," erniedrigt werden");
      for(AufEintragZu::list_t::const_iterator i=L.begin();i!=L.end();++i)
      {
        if(i->AEB.Id()==dispo_auftrag_id)
         {
           mengen_t M=min(i->Menge,S-getRestStk());
           // warum nicht MengeAendern?
           M=AufEintrag(i->AEB).MengeAendern(-M,false,*this);
           S-=M;
         }
      }
  }
}

